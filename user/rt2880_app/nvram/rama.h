#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/route.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>


#define RM_PORT			7654
#define RM_MAX_MSG_LEN		128
#define RM_MAX_NAME_LEN		32 //length of login account
#define RM_MAX_SSID_LEN		32
#define RM_NONCE_LEN		8


/** RM message general format:
 *
 *  <--- RM_HDR_LEN ---> <------ RMM_LEN(m) ------->
 *  <---------------- (m->h->len) ----------------->
 * +--------------------+---------------------------+
 * |                    | payload (TLV data):       |
 * | rm_hdr:            |+--------+ +--------+ +---+|
 * |   len, type, magic || rm_tlv | | rm_tlv | |...||
 * |                    |+--------+ +--------+ +---+|
 * +--------------------+---------------------------+
 *                      ^                           ^
 *  RMM_TLV(m) ---------|                           |
 *  RMM_TAIL(m) ------------------------------------|
 */
typedef struct rm_hdr {
	int32_t len;    //payload length
	int32_t type;
	int32_t magic;
} rm_hdr;
typedef struct rm_msg {
	rm_hdr *h;
	//payload here
} rm_msg;
#define RM_MAGIC		0x5241636f
#define RM_HDR_LEN		sizeof(rm_hdr)
#define RMM_LEN(m)		((m)->h->len - RM_HDR_LEN)
#define RMM_TLV(m)		((rm_tlv *)((m)->h + 1))
#define RMM_TAIL(m)		((rm_tlv *)(((void *)(m->h)) + (m)->h->len))


/** RM TLV data format:
 *
 *  <-RM_TLV_HDR_LEN-> <-RMT_LEN(t)->
 *  <----------- (t->l) ------------>
 * +------------------+-------------++-------------+
 * | rm_tlv_hdr:      | value:      || next rm_tlv |
 * |         t and l  |          v  ||             |
 * +------------------+-------------++-------------+
 *                                  ^
 * RMT_NEXT(t,len) -----------------|
 */
typedef struct rm_tlv {
	int32_t l;   //length of v
	int32_t t;   //type
	char v[0];   //value
} rm_tlv;
#define RM_TLV_HDR_LEN		(2 * sizeof(int32_t))
#define RMT_LEN(t)		((t)->l - RM_TLV_HDR_LEN)
#define RMT_OK(t,len)		((len) >= (int)RM_TLV_HDR_LEN && \
                                 (t)->l >= RM_TLV_HDR_LEN && \
                                 (t)->l <= (len))
#define RMT_NEXT(t,len)		((len) -= (t)->l, \
				 (rm_tlv *)(((void *)(t)) + (t)->l))


/** Information query transaction:
 *         | RM_QUERY --> |
 *  client | <--- RM_INFO | server
 *
 ** RM_QUERY message:
 * +-----------------+--------------+
 * |rm_hdr:          |+------------+|
 * | len             ||t= RM_Q_ALL ||
 * | type= RM_QUERY  ||l= 0        ||
 * | magic= RM_MAGIC |+------------+|
 * +-----------------+--------------+
 */
#define RM_QUERY		0x01
#define RM_Q_ALL		0x02


/** Login procedure:
 *         | RM_LOGIN ------> |
 *         | <-- RM_CHALLENGE |
 *  client | RM_PASSWD -----> | server
 *         | <----- RM_STATUS |
 *
 ** RM_LOGIN message:
 * +-----------------+----------------------------+
 * |rm_hdr:          |+------------+-------------+|
 * | len             ||t= RM_L_SID |v= server id ||
 * | type= RM_LOGIN  ||l= 6        |  (mac addr) ||
 * | magic= RM_MAGIC |+------------+-------------+|
 * +-----------------+----------------------------+
 *
 ** RM_CHALLENGE message:
 * +--------------------+----------------+
 * |rm_hdr:             |+--------------+|
 * | len                ||t= RM_L_NONCE ||
 * | type= RM_CHALLENGE ||l= nonce      ||
 * | magic= RM_MAGIC    |+--------------+|
 * +--------------------+----------------+
 *
 ** RM_PASSWD message:
 * +-----------------+---------------+
 * |rm_hdr:          |+-------------+|
 * | len             ||t= RM_L_HASH ||
 * | type= RM_PASSWD ||l= hash      ||
 * | magic= RM_MAGIC |+-------------+|
 * +-----------------+---------------+
 *
 ** RM_STATE success message for login:
 * +-----------------+--------------------------------------------+
 * |rm_hdr:          |+----------------+ +------------+----------+|
 * | len             ||t= RM_S_SUCCESS | |t= RM_I_CID |v= client ||
 * | type= RM_STATUS ||l= 0            | |l= 6        |       id ||
 * | magic= RM_MAGIC |+----------------+ +------------+----------+|
 * +-----------------+--------------------------------------------+
 */
#define RM_LOGIN		0x11
#define RM_L_SID		0x12
#define RM_CHALLENGE		0x13
#define RM_L_NONCE		0x14
#define RM_PASSWD		0x15
#define RM_L_HASH		0x16


/** IP setting transaction:
 *         | RM_SET_IP --> |
 *  client | <-- RM_STATUS | server
 *
 ** RM_SET_IP message:
 * +-----------------+------------------------------------------------------+
 * |rm_hdr:          |+------------+----------+ +-----------+--------------+|
 * | len             ||t= RM_I_CID |v= client | |t= RM_S_IP |v= ip address ||
 * | type= RM_SET_IP ||l= 6        |       id | |l= 4       |              ||
 * | magic= RM_MAGIC |+------------+----------+ +-----------+--------------+|
 * +-----------------+------------------------------------------------------+
 */
#define RM_SET_IP		0x21


/** SSID setting transaction:
 *         | RM_SET_SSID --> |
 *  client | <---- RM_STATUS | server
 *
 ** RM_SET_SSID message:
 * +-------------------+---------------------------------------------------------+
 * |rm_hdr:            |+------------+----------+ +-------------+---------------+|
 * | len               ||t= RM_I_CID |v= client | |t= RM_S_SSID |v= ssid string ||
 * | type= RM_SET_SSID ||l= 6        |       id | |l= strlen    |               ||
 * | magic= RM_MAGIC   |+------------+----------+ +-------------+---------------+|
 * +-------------------+---------------------------------------------------------+
 */
#define RM_SET_SSID		0x22


/** RM_STATUS message:
 * +-----------------+------------------+    +--------+--------------------------------------+
 * |rm_hdr:          |+----------------+|    |rm_hdr: |+---------------+--------------------+|
 * | len             ||t= RM_S_SUCCESS ||    | len    ||t= RM_S_reason |v= reason string or ||
 * | type= RM_STATUS ||l= 0            || or | type   ||l= strlen      |  other information ||
 * | magic= RM_MAGIC |+----------------+|    | magic  |+---------------+--------------------+|
 * +-----------------+------------------+    +--------+--------------------------------------+
 */
#define RM_STATUS		0xA1
#define RM_S_SUCCESS		0xA2
#define RM_S_UNKNOWN_TLV_TYPE	0xA3
#define RM_S_WRONG_PW		0xA4
#define RM_S_UN_LOGIN		0xA5
#define RM_S_ERROR		0xA6


/* RM_INFO packet:
 * +-----------------+---------------------------------------------------------------------+
 * |rm_hdr:          |+------------+--------+ +-----------+------+ +-------------+--------+|
 * | len             ||t= RM_I_MAC |v= mac  | |t= RM_I_IP |v= ip | |t= RM_I_SSID |v= ssid ||
 * | type= RM_INFO   ||l= 6        |   addr | |l= 4       | addr | |l= strlen    | string ||
 * | magic= RM_MAGIC |+------------+--------+ +-----------+------+ +-------------+--------+|
 * +-----------------+---------------------------------------------------------------------+
 */
#define RM_INFO			0xB1
#define RM_I_MAC		0xB2
#define RM_I_IP			0xB3
#define RM_I_SSID		0xB4
#define RM_I_CID		0xB5 //client id



int rm_append_tlv(rm_msg *m, int type, int length, void *value);
int rm_msg_init(rm_msg *m, int type);
void rm_msg_fini(rm_msg *m);
int rm_msg_check(rm_msg *m);
void rm_print_msg(rm_msg *m);
int md5_hash(char *nonce, char *user, char *pw, unsigned char hash[16]);



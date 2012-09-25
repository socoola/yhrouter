#include <stdio.h>

#include "rama.h"
#include "md5.h"

/*
 * 'length' is the length of 'value'
 */
int rm_append_tlv(rm_msg *m, int type, int length, void *value)
{
	int mlen;
	rm_tlv *tlv;

	if (rm_msg_check(m) != 0)
		return -1;

	mlen = m->h->len + RM_TLV_HDR_LEN + length;
	if (mlen > RM_MAX_MSG_LEN) {
		fprintf(stderr, "MAX msg length exceeded! increase RM_MAX_MSG_LEN\n");
		return -1;
	}
	if (NULL == (m->h = (rm_hdr *)realloc(m->h, mlen)))
		return -1;

	tlv = RMM_TAIL(m);
	tlv->l = RM_TLV_HDR_LEN + length;
	tlv->t = type;
	if (length > 0 && NULL != value)
		memcpy(tlv->v, value, length);
	m->h->len = mlen;

	return 0;
}

int rm_msg_init(rm_msg *m, int type)
{
	m->h = (rm_hdr *)malloc(RM_HDR_LEN);
	if (NULL == m->h)
		return -1;

	m->h->len = RM_HDR_LEN;
	m->h->type = type;
	m->h->magic = RM_MAGIC;

	return 0;
}

void rm_msg_fini(rm_msg *m)
{
	free(m->h);
}

int rm_msg_check(rm_msg *m)
{
	if (m == NULL || m->h == NULL ||
			m->h->magic != RM_MAGIC ||
			m->h->len < RM_HDR_LEN
	   )
		return -1;

	return 0;
}

/*
int rm_init_tlv(rm_tlv *tlv, int type, int length, void *value)
{
}
*/

void rm_print_msg(rm_msg *m)
{
	rm_tlv *tlv;
	int len, i;

	if (NULL == m->h)
		return;

	printf("  hdr ==> ");
	switch (m->h->type) {
		case RM_QUERY: printf("RM_QUERY"); break;
		case RM_LOGIN: printf("RM_LOGIN"); break;
		case RM_CHALLENGE: printf("RM_CHALLENGE"); break;
		case RM_PASSWD: printf("RM_PASSWD"); break;
		case RM_SET_IP: printf("RM_SET_IP"); break;
		case RM_SET_SSID: printf("RM_SET_SSID"); break;
		case RM_STATUS: printf("RM_STATUS"); break;
		case RM_INFO: printf("RM_INFO"); break;
		default:
			printf("type:0x%x", m->h->type); break;
	}
	printf(" len:%d magic:%c\n", m->h->len, (m->h->magic == RM_MAGIC)? 'Y':'N');

	tlv = RMM_TLV(m);
	len = RMM_LEN(m);
	while (RMT_OK(tlv, len)) {
		printf("    tlv => ");
		switch (tlv->t) {
			case RM_Q_ALL: printf("RM_Q_ALL"); break;
			case RM_L_SID: printf("RM_L_SID"); break;
			case RM_L_NONCE: printf("RM_L_NONCE"); break;
			case RM_L_HASH: printf("RM_L_HASH"); break;
			case RM_S_SUCCESS: printf("RM_S_SUCCESS"); break;
			case RM_S_UNKNOWN_TLV_TYPE: printf("RM_S_UNKNOWN_TLV_TYPE"); break;
			case RM_S_WRONG_PW: printf("RM_S_WRONG_PW"); break;
			case RM_S_UN_LOGIN: printf("RM_S_UN_LOGIN"); break;
			case RM_S_ERROR: printf("RM_S_ERROR"); break;
			case RM_I_MAC: printf("RM_I_MAC"); break;
			case RM_I_IP: printf("RM_I_IP"); break;
			case RM_I_SSID: printf("RM_I_SSID"); break;
			case RM_I_CID: printf("RM_I_CID"); break;
			default:
				printf("t:0x%x", tlv->t);
		}
		printf(" l:%d v:", tlv->l);

		if (RMT_LEN(tlv) != 0) {
			printf("0x");
			for (i = 0; i < RMT_LEN(tlv); i++) {
				printf("%02x ", tlv->v[i] & 0xff);
			}
		}
		else
			printf("-");
		printf("\n");
		tlv = RMT_NEXT(tlv, len);
	}
}

int md5_hash(char *nonce, char *user, char *pw, unsigned char hash[16])
{
	MD5_CONTEXT md5ctx;
	char *md5in, *p;
	int md5len;

	if (nonce == NULL || user == NULL || pw == NULL || hash == NULL)
		return -1;
	md5len = RM_NONCE_LEN + strlen(user) + strlen(pw);
	md5in = (char *)malloc(md5len);
	if (md5in == NULL)
		return -1;

	memcpy(md5in, nonce, RM_NONCE_LEN);
	p = md5in + RM_NONCE_LEN;
	strncpy(p, user, strlen(user));
	p += strlen(user);
	strncpy(p, pw, strlen(pw));

	MD5Init(&md5ctx);
	MD5Update(&md5ctx, (unsigned char *)md5in, md5len);
	MD5Final(hash, &md5ctx);
	free(md5in);
	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //add by zsf 2010-10-29

#include <sys/sem.h>
#include <sys/ipc.h>
#include <errno.h>



#include "nvram.h"
#include "nvram_env.h"
#include "flash_api.h"




#include <pthread.h> //add by zsf 2010-10-28

char libnvram_debug = 0;
#define LIBNV_PRINT(x, ...) do { if (libnvram_debug) printf("%s %d: " x, __FILE__, __LINE__, ## __VA_ARGS__); } while(0)
#define LIBNV_ERROR(x, ...) do { printf("%s %d: ERROR! " x, __FILE__, __LINE__, ## __VA_ARGS__); } while(0)


typedef struct environment_s {
	unsigned long crc;		//CRC32 over data bytes
	char *data;
	unsigned long verify_crc;		//CRC32 over data bytes
	char *verify_data;
} env_t;

typedef struct cache_environment_s {
	char *name;
	char *value;
} cache_t;
//modify by zsf 2010-12-9
#define MAX_CACHE_ENTRY 600
//#define MAX_CACHE_ENTRY 300
typedef struct block_s {
	char *name;
	env_t env;			//env block
	cache_t	cache[MAX_CACHE_ENTRY];	//env cache entry by entry
	unsigned long flash_offset;
	unsigned long flash_max_len;	//ENV_BLK_SIZE

	char valid;
	char dirty;
} block_t;





/*
#ifdef CONFIG_DUAL_IMAGE
#define FLASH_BLOCK_NUM	4
#else
#define FLASH_BLOCK_NUM	3
#endif

static block_t fb[FLASH_BLOCK_NUM] =
{
#ifdef CONFIG_DUAL_IMAGE
	{
		.name = "uboot",
		.flash_offset =  0x0,
		.flash_max_len = ENV_UBOOT_SIZE,
		.valid = 0
	},
#endif
	{
		.name = "2860",
		.flash_offset =  0x2000,
		.flash_max_len = ENV_BLK_SIZE,
		.valid = 0
	},
	{
		.name = "inic",
		.flash_offset = 0x6000,
		.flash_max_len = ENV_BLK_SIZE,
		.valid = 0
	},
	{
		.name = "2561",
		.flash_offset = 0xa000,
		.flash_max_len = ENV_BLK_SIZE,
		.valid = 0
	}
};
*/


#ifdef CONFIG_DUAL_IMAGE
#define FLASH_BLOCK_NUM	5
#else
#define FLASH_BLOCK_NUM	4
#endif

static block_t fb[FLASH_BLOCK_NUM] =
{

#ifdef CONFIG_DUAL_IMAGE
	{
		.name = "uboot",
		.flash_offset =  0x0,
		.flash_max_len = ENV_UBOOT_SIZE,
		.valid = 0
	},
#endif
	{
		.name = "2860",
		.flash_offset =  0x2000,
		.flash_max_len = ENV_BLK_SIZE,
		.valid = 0
	},
	{
		.name = "inic",
		.flash_offset = 0x6000,
		.flash_max_len = ENV_BLK_SIZE,
		.valid = 0
	},
	{
		.name = "2561",
		.flash_offset = 0xa000,
		.flash_max_len = ENV_BLK_SIZE,
		.valid = 0
	},
	{//second block of flash (deffrent block from other)
		.name = "2860_bak",
		.flash_offset =  0x10000,
		.flash_max_len = ENV_BLK_SIZE,
		.valid = 0
	}
};


//pthread_mutex_t flash_mutex=PTHREAD_MUTEX_INITIALIZER; //add by zsf 2010-10-28

union semun {
 int val;
 struct semid_ds *buf;
 unsigned short *array;
} arg;
int sem_creat(key_t key)
{
 union semun sem;
 int semid;
 sem.val = 0;
 semid = semget(key,1,IPC_CREAT|0666);
 if (-1 == semid){
  printf("create semaphore error:%s-%d\n", strerror(errno), key);
  exit(-1);
 }
 semctl(semid,0,SETVAL,sem);
 return semid;
}
void del_sem(int semid)
{
 union semun sem;
 sem.val = 0;
 semctl(semid,0,IPC_RMID,sem);
}

int sem_p(int semid)
{
 struct sembuf sops={0,+1,IPC_NOWAIT};
 return (semop(semid,&sops,1));
}
int sem_v(int semid)
{
 struct sembuf sops={0,-1,IPC_NOWAIT};
 return (semop(semid,&sops,1));
}




int new_sem(key_t key)
{
 union semun sem;
 int semid;
 sem.val = 0;
 semid = semget(key,0,0);
 if (-1 ==  semid){
  printf("new semaphore error:%s-%d\n", strerror(errno), key);
  exit(-1);
 }
 return semid;
}




void wait_v(int semid)
{
 struct sembuf sops={0,0,0};
 semop(semid,&sops,1);
}





void init_flash_sem(void)
{
	key_t key;	
	int semid;
//	struct semid_ds buf;
	key = ftok("/",0);
	semid = sem_creat(key);
}


//x is the value returned if the check failed
#define LIBNV_CHECK_INDEX(x) do { \
	if (index < 0 || index >= FLASH_BLOCK_NUM) { \
		LIBNV_PRINT("index(%d) is out of range\n", index); \
		return x; \
	} \
} while (0)

#define LIBNV_CHECK_VALID() do { \
	if (!fb[index].valid) { \
		LIBNV_PRINT("fb[%d] invalid, init again\n", index); \
		nvram_init(index); \
	} \
} while (0)

/*
#define debug_LIBNV_CHECK_VALID() do { \
	if (!fb[index].valid) { \
		LIBNV_PRINT("fb[%d] invalid, init again\n", index); \
		debug_nvram_init(index); \
	} \
} while (0)
*/

#define FREE(x) do { if (x != NULL) {free(x); x=NULL;} } while(0)

/*
 * 1. read env from flash
 * 2. parse entries
 * 3. save the entries to cache
 */

#define DATA_MEM 0
#define VERIFY_DATA_MEM 1
void save_2860_bak(int index_used,int to_block,int flag)
{
	int count_bak=0;
	int to,len;

	sleep(1);

if(flag==DATA_MEM){


		to = fb[to_block].flash_offset;
		len  = sizeof(fb[index_used].env.crc);
		count_bak=0;
		while(flash_write((char *)&fb[index_used].env.crc, to, len)<0){
			printf("write to backup crc: write crc to flash backup error count=%d",count_bak);
			if(++count_bak>10){
				printf("write crc to flash backup error");
				break;
			}	
			sleep(1);
		}


		to = to+len;
		len = fb[index_used].flash_max_len - len;
		count_bak=0;

		while(flash_write(fb[index_used].env.data, to, len)<0){
			printf("write to backup data: write data to flash backup error count=%d",count_bak);
			if(++count_bak>10){
				printf("write data to flash backup error");
				break;
			}	
			sleep(1);
		}

}else if(flag==VERIFY_DATA_MEM){
		to = fb[to_block].flash_offset;
		len  = sizeof(fb[index_used].env.crc);
		count_bak=0;
		while(flash_write((char *)&fb[index_used].env.verify_crc, to, len)<0){
			printf("write to backup crc: write crc to flash backup error count=%d",count_bak);
			if(++count_bak>10){
				printf("write crc to flash backup error");
				break;
			}	
			sleep(1);
		}


		to = to+len;
		len = fb[index_used].flash_max_len - len;
		count_bak=0;

		while(flash_write(fb[index_used].env.verify_data, to, len)<0){
			printf("write to backup data: write data to flash backup error count=%d",count_bak);
			if(++count_bak>10){
				printf("write data to flash backup error");
				break;
			}	
			sleep(1);
		}
}

		

		
}
void read_2860_bak(int index_used,int from_block)
{

	int count_read=0;
	int from,len;




	//read crc from flash backup
	from = fb[from_block].flash_offset;
	len  = sizeof(fb[index_used].env.crc);
	count_read=0;
	while(flash_read((char *)&fb[index_used].env.crc, from, len)<0){
		printf("read backup crc:backup crc error count=%d\n",count_read);
		if(++count_read>1){
			break;
		}
		sleep(1);
	}


	//read data from flash
	from = from + len;
	len = fb[index_used].flash_max_len - len;

	fb[index_used].env.data = (char *)malloc(len);

	
	bzero(fb[index_used].env.data, len);
	if(count_read<=1){
		count_read=0;
		while(flash_read(fb[index_used].env.data, from, len)<0){
			printf("read backup data:read data error count=%d\n",count_read);
			if(++count_read>1){
				break; 
			}
			sleep(1);
		}
	}

}






void nvram_init(int index)
{
	unsigned long from;
	unsigned long count=0;
	int i, len;
	char *p, *q;

	LIBNV_PRINT("--> nvram_init %d\n", index);
	LIBNV_CHECK_INDEX();
	if (fb[index].valid)
		return;
//pthread_mutex_lock(&flash_mutex); //add by zsf 2010-10-28
	





        key_t key;
        int semid;
         
        key=ftok("/",0);
        semid=new_sem(key);
        wait_v(semid);
        sem_p(semid);

	






	//read crc from flash
	from = fb[index].flash_offset;
	len = sizeof(fb[index].env.crc);
	count=0;
	while(flash_read((char *)&fb[index].env.crc, from, len)<0){
		printf("nvram_init:read crc error count=%d\n",count);
		if(++count>1){
			break;
		}
		sleep(1);
	}

	//read data from flash
	from = from + len;
	len = fb[index].flash_max_len - len;
	fb[index].env.data = (char *)malloc(len);
	bzero(fb[index].env.data, len);
	if(count<=1){
		count=0;
		while(flash_read(fb[index].env.data, from, len)<0){
			printf("nvram_init:read data error count=%d\n",count);
			if(++count>1){
				break; 
			}
			sleep(1);
		}
	}




	

	//check crc
	//printf("crc shall be %08lx\n", crc32(0, (unsigned char *)fb[index].env.data, len));
	if (crc32(0, (unsigned char *)fb[index].env.data, len) != fb[index].env.crc) {
		LIBNV_PRINT("Bad CRC %x, ignore values in flash.\n", fb[index].env.crc);
		FREE(fb[index].env.data);

		fb[index].valid = 0;
		fb[index].dirty = 0;
		
		read_2860_bak(index,RT2860_NVRAM_BAK); //read 2860_backup flash to 2860 fb[index]


		if (crc32(0, (unsigned char *)fb[index].env.data, len) != fb[index].env.crc) {
			fb[index].valid = 0;
			FREE(fb[index].env.data);
//pthread_mutex_unlock(&flash_mutex); //add by zsf 2010-10-28
sem_v(semid);
			return;	
		}else{

			save_2860_bak(index,index,DATA_MEM);//save 2860 fb[index] to 2860_backup flash
		
			fb[index].valid = 1;
		}

		
	}






	//parse env to cache
	p = fb[index].env.data;
	for (i = 0; i < MAX_CACHE_ENTRY; i++) {
		if (NULL == (q = strchr(p, '='))) {
			LIBNV_PRINT("parsed failed - cannot find '='\n");
			break;
		}
		*q = '\0'; //strip '='
		fb[index].cache[i].name = strdup(p);
		//printf("  %d '%s'->", i, p);

		p = q + 1; //value
		if (NULL == (q = strchr(p, '\0'))) {
			LIBNV_PRINT("parsed failed - cannot find '\\0'\n");
			break;
		}
		fb[index].cache[i].value = strdup(p);
		//printf("'%s'\n", p);

		p = q + 1; //next entry
		if (p - fb[index].env.data + 1 >= len) //end of block
			break;
		if (*p == '\0') //end of env
			break;
	}
	if (i == MAX_CACHE_ENTRY)
		LIBNV_PRINT("run out of env cache, please increase MAX_CACHE_ENTRY\n");

	FREE(fb[index].env.data); //free it to save momery
	fb[index].valid = 1;
	fb[index].dirty = 0;
//pthread_mutex_unlock(&flash_mutex); //add by zsf 2010-10-28
sem_v(semid);
}



/*
void debug_nvram_init(int index)
{
	unsigned long from;
	unsigned long count=0;
	int i, len;
	char *p, *q;

	printf("--> debug_nvram_init %d\n", index);
	LIBNV_CHECK_INDEX();
	if (fb[index].valid)
	{		
		printf("debug nvram init:  valid=1,return.\n");
		return;
	}
//pthread_mutex_lock(&flash_mutex); //add by zsf 2010-10-2
        key_t key;
        int semid;

        key=ftok("/",0);
        semid=new_sem(key);
        wait_v(semid);
        sem_p(semid);

printf("debug nvram init: sem  lock\n");





	//read crc from flash
	from = fb[index].flash_offset;
	len = sizeof(fb[index].env.crc);
	count=0;
	while(flash_read((char *)&fb[index].env.crc, from, len)<0){
		printf("nvram_init:read crc error count=%d\n",count);
		if(++count>10){
			break;
		}
		sleep(1);
	}

	//read data from flash
	from = from + len;
	len = fb[index].flash_max_len - len;
	fb[index].env.data = (char *)malloc(len);
	bzero(fb[index].env.data, len);
	if(count<=10){
		count=0;
		while(flash_read(fb[index].env.data, from, len)<0){
			printf("nvram_init:read data error count=%d\n",count);
			if(++count>10){
				break; 
			}
			sleep(1);
		}
	}

	//check crc
	//printf("crc shall be %08lx\n", crc32(0, (unsigned char *)fb[index].env.data, len));
	if (crc32(0, (unsigned char *)fb[index].env.data, len) != fb[index].env.crc) {
		printf("Bad CRC %x,read again.\n", fb[index].env.crc);
		 FREE(fb[index].env.data);
		//empty cache
//		fb[index].valid = 1;
		fb[index].valid = 0;
		fb[index].dirty = 0;


		//retry add by zsf 2010-10-29
		//read crc from flash
		sleep(4);
		from = fb[index].flash_offset;
		len = sizeof(fb[index].env.crc);
		flash_read((char *)&fb[index].env.crc, from, len);

		//read data from flash
		from = from + len;
		len = fb[index].flash_max_len - len;
		fb[index].env.data = (char *)malloc(len);
		bzero(fb[index].env.data, len);
		flash_read(fb[index].env.data, from, len);

		if (crc32(0, (unsigned char *)fb[index].env.data, len) != fb[index].env.crc) {
			fb[index].valid = 0;
			FREE(fb[index].env.data);
printf("debug nvram init:second read crc error\n");
printf("debug nvram init:sem  unlock\n");
//pthread_mutex_unlock(&flash_mutex); //add by zsf 2010-10-28
sem_v(semid);

			return;	
		}else{
			fb[index].valid = 1;
		}
	}






	//parse env to cache
	p = fb[index].env.data;
	for (i = 0; i < MAX_CACHE_ENTRY; i++) {
		if (NULL == (q = strchr(p, '='))) {
			LIBNV_PRINT("parsed failed - cannot find '='\n");
			break;
		}
		*q = '\0'; //strip '='
		fb[index].cache[i].name = strdup(p);
		//printf("  %d '%s'->", i, p);

		p = q + 1; //value
		if (NULL == (q = strchr(p, '\0'))) {
			LIBNV_PRINT("parsed failed - cannot find '\\0'\n");
			break;
		}
		fb[index].cache[i].value = strdup(p);
		//printf("'%s'\n", p);

		p = q + 1; //next entry
		if (p - fb[index].env.data + 1 >= len) //end of block
			break;
		if (*p == '\0') //end of env
			break;
	}
	if (i == MAX_CACHE_ENTRY)
		LIBNV_PRINT("run out of env cache, please increase MAX_CACHE_ENTRY\n");

	FREE(fb[index].env.data); //free it to save momery
	fb[index].valid = 1;
	fb[index].dirty = 0;
printf("debug nvram init:sem end unlock\n");
//pthread_mutex_unlock(&flash_mutex); //add by zsf 2010-10-28
sem_v(semid);
}


*/








void nvram_close(int index)
{
	int i;

	LIBNV_PRINT("--> nvram_close %d\n", index);
	LIBNV_CHECK_INDEX();
	if (!fb[index].valid)
		return;
	if (fb[index].dirty)
		nvram_commit(index);

	//free env
	FREE(fb[index].env.data);

	//free cache
	for (i = 0; i < MAX_CACHE_ENTRY; i++) {
		FREE(fb[index].cache[i].name);
		FREE(fb[index].cache[i].value);
	}

	fb[index].valid = 0;
}

/*
 * return idx (0 ~ iMAX_CACHE_ENTRY)
 * return -1 if no such value or empty cache
 */
static int cache_idx(int index, char *name)
{
	int i;

	for (i = 0; i < MAX_CACHE_ENTRY; i++) {
		if (!fb[index].cache[i].name)
			return -1;
		if (!strcmp(name, fb[index].cache[i].name))
			return i;
	}
	return -1;
}

char *nvram_get(int index, char *name)
{
	//LIBNV_PRINT("--> nvram_get\n");
	//removed by zsf 2010-10-29
//	nvram_close(index);
//	nvram_init(index);
	return nvram_bufget(index, name);
}

int nvram_set(int index, char *name, char *value)
{
	//LIBNV_PRINT("--> nvram_set\n");
	if (-1 == nvram_bufset(index, name, value))
		return -1;
	return nvram_commit(index);
}
/*
int debug_nvram_set(int index, char *name, char *value)
{
	printf("--> debug_nvram_set\n");
	printf("--> debug_nvram_bufset\n");
	if (-1 == debug_nvram_bufset(index, name, value))
		return -1;
	printf("--> debug_nvram_commit\n");
	return debug_nvram_commit(index);
}
*/
char *nvram_bufget(int index, char *name)
{
	int idx;
	static char *ret;

	//LIBNV_PRINT("--> nvram_bufget %d\n", index);
	LIBNV_CHECK_INDEX("");
	LIBNV_CHECK_VALID();
	LIBNV_CHECK_VALID(); //add by zsf 2010-10-29
	LIBNV_CHECK_VALID(); //add by zsf 2010-10-29
	idx = cache_idx(index, name);

	if (-1 != idx) {
		if (fb[index].cache[idx].value) {
			//duplicate the value in case caller modify it
			ret = strdup(fb[index].cache[idx].value);
			LIBNV_PRINT("bufget %d '%s'->'%s'\n", index, name, ret);
			return ret;
		}
	}

	//no default value set?
	//btw, we don't return NULL anymore!
	LIBNV_PRINT("bufget %d '%s'->''(empty) Warning!\n", index, name);
	return "";
}

/*
char *debug_nvram_bufget(int index, char *name)
{
	int idx;
	static char *ret;

	//LIBNV_PRINT("--> nvram_bufget %d\n", index);
	LIBNV_CHECK_INDEX("");
	debug_LIBNV_CHECK_VALID();
	debug_LIBNV_CHECK_VALID(); //add by zsf 2010-10-29
	debug_LIBNV_CHECK_VALID(); //add by zsf 2010-10-29
	idx = cache_idx(index, name);

	if (-1 != idx) {
		if (fb[index].cache[idx].value) {
			//duplicate the value in case caller modify it
			ret = strdup(fb[index].cache[idx].value);
			LIBNV_PRINT("bufget %d '%s'->'%s'\n", index, name, ret);
			return ret;
		}
	}

	//no default value set?
	//btw, we don't return NULL anymore!
	printf("bufget %d '%s'->''(empty) Warning!\n", index, name);
	
	return "";
}
*/

int nvram_bufset(int index, char *name, char *value)
{
	int idx;

	//LIBNV_PRINT("--> nvram_bufset\n");
	LIBNV_CHECK_INDEX(-1);
	LIBNV_CHECK_VALID();
	LIBNV_CHECK_VALID(); //add by zsf 2010-10-29
	LIBNV_CHECK_VALID(); //add by zsf 2010-10-29
	idx = cache_idx(index, name);

	if (-1 == idx) {
		//find the first empty room
		for (idx = 0; idx < MAX_CACHE_ENTRY; idx++) {
			if (!fb[index].cache[idx].name)
				break;
		}
		//no any empty room
		if (idx == MAX_CACHE_ENTRY) {
			LIBNV_ERROR("run out of env cache, please increase MAX_CACHE_ENTRY\n");
			return -1;
		}
		fb[index].cache[idx].name = strdup(name);
		fb[index].cache[idx].value = strdup(value);
	}
	else {
		//abandon the previous value
		FREE(fb[index].cache[idx].value);
		fb[index].cache[idx].value = strdup(value);
	}
	LIBNV_PRINT("bufset %d '%s'->'%s'\n", index, name, value);
	fb[index].dirty = 1;
	return 0;
}

/*
int debug_nvram_bufset(int index, char *name, char *value)
{
	int idx;

	//LIBNV_PRINT("--> nvram_bufset\n");
	LIBNV_CHECK_INDEX(-1);
	debug_LIBNV_CHECK_VALID();
	debug_LIBNV_CHECK_VALID(); //add by zsf 2010-10-29
	debug_LIBNV_CHECK_VALID(); //add by zsf 2010-10-29
	idx = cache_idx(index, name);

	if (-1 == idx) {
		//find the first empty room
		for (idx = 0; idx < MAX_CACHE_ENTRY; idx++) {
			if (!fb[index].cache[idx].name)
				break;
		}
		//no any empty room
		if (idx == MAX_CACHE_ENTRY) {
			LIBNV_ERROR("run out of env cache, please increase MAX_CACHE_ENTRY\n");
			return -1;
		}
		fb[index].cache[idx].name = strdup(name);
		fb[index].cache[idx].value = strdup(value);
	}
	else {
		//abandon the previous value
		FREE(fb[index].cache[idx].value);
		fb[index].cache[idx].value = strdup(value);
	}
	printf("bufset %d '%s'->'%s'\n", index, name, value);
	fb[index].dirty = 1;
	printf("debug_nvram_bufset: dirty=1,return");
	return 0;
}
*/
void nvram_buflist(int index)
{
	int i;

	//LIBNV_PRINT("--> nvram_buflist %d\n", index);
	LIBNV_CHECK_INDEX();
	LIBNV_CHECK_VALID();

	for (i = 0; i < MAX_CACHE_ENTRY; i++) {
		if (!fb[index].cache[i].name)
			break;
		printf("  '%s'='%s'\n", fb[index].cache[i].name, fb[index].cache[i].value);
	}
}

/*
 * write flash from cache
 */


/*
int nvram_commit(int index)
{
	unsigned long to,from;
	int i, len;
	char *p;

	//LIBNV_PRINT("--> nvram_commit %d\n", index);
	LIBNV_CHECK_INDEX(-1);
	LIBNV_CHECK_VALID();

	if (!fb[index].dirty) {
		LIBNV_PRINT("nothing to be committed\n");
		return 0;
	}

pthread_mutex_lock(&flash_mutex); //add by zsf 2010-10-28
	//construct env block
	len = fb[index].flash_max_len - sizeof(fb[index].env.crc);
	fb[index].env.data = (char *)malloc(len);
	bzero(fb[index].env.data, len);
	p = fb[index].env.data;
	for (i = 0; i < MAX_CACHE_ENTRY; i++) {
		int l;
		if (!fb[index].cache[i].name || !fb[index].cache[i].value)
			break;
		l = strlen(fb[index].cache[i].name) + strlen(fb[index].cache[i].value) + 2;
		if (p - fb[index].env.data + 2 >= ENV_BLK_SIZE) {
			LIBNV_ERROR("ENV_BLK_SIZE 0x%x is not enough!", ENV_BLK_SIZE);
			FREE(fb[index].env.data);
pthread_mutex_unlock(&flash_mutex); ////add by zsf 2010-10-28
			return -1;
		}
		snprintf(p, l, "%s=%s", fb[index].cache[i].name, fb[index].cache[i].value);
		p += l;
	}
	*p = '\0'; //ending null





	//calculate crc
	fb[index].env.crc = (unsigned long)crc32(0, (unsigned char *)fb[index].env.data, len);
	printf("Commit crc = %x\n", (unsigned int)fb[index].env.crc);

	//write crc to flash
	to = fb[index].flash_offset;
	len = sizeof(fb[index].env.crc);
	flash_write((char *)&fb[index].env.crc, to, len);

	//write data to flash
	to = to + len;
	len = fb[index].flash_max_len - len;
	flash_write(fb[index].env.data, to, len);
	//FREE(fb[index].env.data);

	fb[index].dirty = 0;






	//verify data in flash after write to flash 
	//add by zsf 2010-10-29

     //read crc from flash
        from = fb[index].flash_offset;
        len = sizeof(fb[index].env.verify_crc);
        flash_read((char *)&fb[index].env.verify_crc, from, len);

        //read data from flash
        from = from + len;
        len = fb[index].flash_max_len - len;
        fb[index].env.verify_data = (char *)malloc(len);
	bzero(fb[index].env.verify_data, len);
        flash_read(fb[index].env.verify_data, from, len);



        if (crc32(0, (unsigned char *)fb[index].env.verify_data, len) != fb[index].env.verify_crc) {
                LIBNV_PRINT("Bad CRC %x, rewrite para to  flash.\n", fb[index].env.verify_crc);
               	FREE(fb[index].env.verify_data);

	//	sleep(1);


			//calculate crc
		fb[index].env.crc = (unsigned long)crc32(0, (unsigned char *)fb[index].env.data, len);
		printf("second time Commit crc = %x\n", (unsigned int)fb[index].env.crc);

		//write crc to flash
		to = fb[index].flash_offset;
		len = sizeof(fb[index].env.crc);
		flash_write((char *)&fb[index].env.crc, to, len);

		//write data to flash
		to = to + len;
		len = fb[index].flash_max_len - len;
		flash_write(fb[index].env.data, to, len);

		


	     //read crc from flash
		from = fb[index].flash_offset;
		len = sizeof(fb[index].env.verify_crc);
		flash_read((char *)&fb[index].env.verify_crc, from, len);

		//read data from flash
		from = from + len;
		len = fb[index].flash_max_len - len;
		fb[index].env.verify_data = (char *)malloc(len);
		bzero(fb[index].env.verify_data, len);
		flash_read(fb[index].env.verify_data, from, len);

		 if (crc32(0, (unsigned char *)fb[index].env.verify_data, len) != fb[index].env.verify_crc) {
            	   	 printf("Bad CRC %x, second time write para to  flash.\n", fb[index].env.verify_crc);
			fb[index].dirty = 1;
		}	
        }

	FREE(fb[index].env.verify_data);
	FREE(fb[index].env.data);



pthread_mutex_unlock(&flash_mutex); //add by zsf 2010-10-28
	return 0;
}
*/





int nvram_commit(int index)
{
	unsigned long to,count=0,from,count_bak=0;
	int i, len;
	char *p;

	//LIBNV_PRINT("--> nvram_commit %d\n", index);
	LIBNV_CHECK_INDEX(-1);
	LIBNV_CHECK_VALID();

	if (!fb[index].dirty) {
		LIBNV_PRINT("nothing to be committed\n");
		return 0;
	}

//pthread_mutex_lock(&flash_mutex); //add by zsf 2010-10-28
        key_t key;
        int semid;

        key=ftok("/",0);
        semid=new_sem(key);
        wait_v(semid);
        sem_p(semid);



	//construct env block
	len = fb[index].flash_max_len - sizeof(fb[index].env.crc);
	fb[index].env.data = (char *)malloc(len);
	bzero(fb[index].env.data, len);
	p = fb[index].env.data;
	for (i = 0; i < MAX_CACHE_ENTRY; i++) {
		int l;
		if (!fb[index].cache[i].name || !fb[index].cache[i].value)
			break;
		l = strlen(fb[index].cache[i].name) + strlen(fb[index].cache[i].value) + 2;
		if (p - fb[index].env.data + 2 >= ENV_BLK_SIZE) {
			LIBNV_ERROR("ENV_BLK_SIZE 0x%x is not enough!", ENV_BLK_SIZE);
			FREE(fb[index].env.data);
//pthread_mutex_unlock(&flash_mutex); ////add by zsf 2010-10-28
sem_v(semid);
			return -1;
		}
		snprintf(p, l, "%s=%s", fb[index].cache[i].name, fb[index].cache[i].value);
		p += l;
	}
	*p = '\0'; //ending null



	fb[index].dirty = 0;

	//calculate crc
	fb[index].env.crc = (unsigned long)crc32(0, (unsigned char *)fb[index].env.data, len);
	printf("Commit crc = %x\n", (unsigned int)fb[index].env.crc);

	//write crc to flash
	to = fb[index].flash_offset;
	len = sizeof(fb[index].env.crc);
	//flash_write((char *)&fb[index].env.crc, to, len);

	count=0;
	while(flash_write((char *)&fb[index].env.crc, to, len)<0){
		printf("write crc to flash error count=%d",count);
		if(++count>10){
			fb[index].dirty = 1;
			printf("write crc to flash error,dirty=1");
			break;
		}	
		sleep(1);
	}

/*
	count_bak=0;
	while(flash_write((char *)&fb[RT2860_NVRAM_BAK].env.crc, to, len)<0){
		printf("write crc to flash backup error count=%d",count_bak);
		if(++count_bak>10){
			printf("write crc to flash backup error");
			break;
		}	
		sleep(1);
	}
	*/

	
	if(count<=10){
		//write data to flash
		to = to + len;
		len = fb[index].flash_max_len - len;
		count=0;
		while(flash_write(fb[index].env.data, to, len)<0){
			printf("write data to flash error count=%d",count);
			if(count++>10){
				fb[index].dirty = 1;
				printf("write data to flash error,dirty=1");
				break;
			}	
			sleep(1);
		}
	}
/*
	if(count<=10){
		count_bak=0;
		while(flash_write((char *)&fb[RT2860_NVRAM_BAK].env.data, to, len)<0){
			printf("write data to flash backup error count=%d",count_bak);
			if(++count_bak>10){
				printf("write data to flash backup error");
				break;
			}	
			sleep(1);
		}
	}

	*/
	//FREE(fb[index].env.data);




//read and verify  add by zsf 2010-10-30
	
	if(fb[index].dirty == 0){

	
		//read crc from flash
		from = fb[index].flash_offset;
		len = sizeof(fb[index].env.verify_crc);
		count=0;
		while(flash_read((char *)&fb[index].env.verify_crc, from, len)<0){
		        printf("nvram_commit verify:read crc error count=%d\n",count);
		        if(++count>10){
		                break;
		        }
		        sleep(1);
		}

		//read data from flash
		from = from + len;
		len = fb[index].flash_max_len - len;
		fb[index].env.verify_data = (char *)malloc(len);
		bzero(fb[index].env.verify_data, len);
		if(count<=10){
		        count=0;
		        while(flash_read(fb[index].env.verify_data, from, len)<0){
		                printf("nvram_commit verify:read data error count=%d\n",count);
		                if(++count>10){
		                        break;
		                }
		                sleep(1);
		        }

		}
	}else{
		count=0;
		fb[index].env.verify_crc=0;
	}
	



	fb[index].dirty = 0;
	if(count<=10){ //read succeed 
		 if (crc32(0, (unsigned char *)fb[index].env.verify_data, len) != fb[index].env.verify_crc) {
				
				//write crc to flash
				to = fb[index].flash_offset;
				len = sizeof(fb[index].env.crc);
				//flash_write((char *)&fb[index].env.crc, to, len);

				count=0;
				while(flash_write((char *)&fb[index].env.crc, to, len)<0){
					printf("write crc to flash error count=%d",count);
					if(++count>10){
						fb[index].dirty = 1;
						printf("write crc to flash error,dirty=1");
						break;
					}	
					sleep(1);
				}
		
				count_bak=0;
				while(flash_write((char *)&fb[RT2860_NVRAM_BAK].env.crc, to, len)<0){
					printf("write crc to flash backup error count=%d",count_bak);
					if(++count_bak>10){
						printf("write crc to flash backup error");
						break;
					}	
					sleep(1);
				}

				if(count<=10){
					//write data to flash
					to = to + len;
					len = fb[index].flash_max_len - len;
					count=0;
					while(flash_write(fb[index].env.data, to, len)<0){
						printf("write data to flash error count=%d",count);
						if(count++>10){
							fb[index].dirty = 1;
							printf("write data to flash error,dirty=1");
							break;
						}	
						sleep(1);
					}
					count_bak=0;
					while(flash_write((char *)&fb[RT2860_NVRAM_BAK].env.data, to, len)<0){
						printf("write data to flash backup error count=%d",count_bak);
						if(++count_bak>10){
							printf("write data to flash backup error");
							break;
						}	
						sleep(1);
					}

					
				}
	
		}else{
			//crc ok  
			save_2860_bak(index,RT2860_NVRAM_BAK,VERIFY_DATA_MEM);
		}
		 
	}
	FREE(fb[index].env.verify_data); //free it to save momery
	FREE(fb[index].env.data);




//pthread_mutex_unlock(&flash_mutex); //add by zsf 2010-10-28
sem_v(semid);
	return 0;
}





/*
int debug_nvram_commit(int index)
{
	unsigned long to,count=0,from;
	int i, len;
	char *p;

	//LIBNV_PRINT("--> nvram_commit %d\n", index);
	LIBNV_CHECK_INDEX(-1);
	debug_LIBNV_CHECK_VALID();

	if (!fb[index].dirty) {
		printf("nothing to be committed,dirty=0 return\n");
		return 0;
	}

//pthread_mutex_lock(&flash_mutex); //add by zsf 2010-10-28
        key_t key;
        int semid;

        key=ftok("/",0);
        semid=new_sem(key);
        wait_v(semid);
        sem_p(semid);

printf("debug_nvram_commit:sem lock\n");
	//construct env block
	len = fb[index].flash_max_len - sizeof(fb[index].env.crc);
	fb[index].env.data = (char *)malloc(len);
	bzero(fb[index].env.data, len);
	p = fb[index].env.data;
	for (i = 0; i < MAX_CACHE_ENTRY; i++) {
		int l;
		if (!fb[index].cache[i].name || !fb[index].cache[i].value)
			break;
		l = strlen(fb[index].cache[i].name) + strlen(fb[index].cache[i].value) + 2;
		if (p - fb[index].env.data + 2 >= ENV_BLK_SIZE) {
			LIBNV_ERROR("ENV_BLK_SIZE 0x%x is not enough!", ENV_BLK_SIZE);
			FREE(fb[index].env.data);
printf("debug_nvram_commit:1 sem  unlock\n");
//pthread_mutex_unlock(&flash_mutex); ////add by zsf 2010-10-28
sem_v(semid);
			return -1;
		}
		snprintf(p, l, "%s=%s", fb[index].cache[i].name, fb[index].cache[i].value);
		p += l;
	}
	*p = '\0'; //ending null



	fb[index].dirty = 0;

	//calculate crc
	fb[index].env.crc = (unsigned long)crc32(0, (unsigned char *)fb[index].env.data, len);
	printf("Commit crc = %x\n", (unsigned int)fb[index].env.crc);

	//write crc to flash
	to = fb[index].flash_offset;
	len = sizeof(fb[index].env.crc);
	//flash_write((char *)&fb[index].env.crc, to, len);

	count=0;
	while(flash_write((char *)&fb[index].env.crc, to, len)<0){
		printf("write crc to flash error count=%d",count);
		if(++count>10){
			fb[index].dirty = 1;
			printf("write crc to flash error,dirty=1");
			break;
		}	
		sleep(1);
	}

	if(count<=10){
		//write data to flash
		to = to + len;
		len = fb[index].flash_max_len - len;
		count=0;
		while(flash_write(fb[index].env.data, to, len)<0){
			printf("write data to flash error count=%d",count);
			if(count++>10){
				fb[index].dirty = 1;
				printf("write data to flash error,dirty=1");
				break;
			}	
			sleep(1);
		}
	}

	//FREE(fb[index].env.data);




//read and verify  add by zsf 2010-10-30
	
	if(fb[index].dirty == 0){
	 printf("commit:dirty=0  read vrify\n");
	
		//read crc from flash
		from = fb[index].flash_offset;
		len = sizeof(fb[index].env.verify_crc);
		count=0;
		while(flash_read((char *)&fb[index].env.verify_crc, from, len)<0){
		        printf("nvram_commit verify:read crc error count=%d\n",count);
		        if(++count>10){
		                break;
		        }
		        sleep(1);
		}

		//read data from flash
		from = from + len;
		len = fb[index].flash_max_len - len;
		fb[index].env.verify_data = (char *)malloc(len);
		bzero(fb[index].env.verify_data, len);
		if(count<=10){
		        count=0;
		        while(flash_read(fb[index].env.verify_data, from, len)<0){
		                printf("nvram_commit verify:read data error count=%d\n",count);
		                if(++count>10){
		                        break;
		                }
		                sleep(1);
		        }

		}
	}else{
		 printf("commit:dirty=1  read vrify\n");
		count=0;
		fb[index].env.verify_crc=0;
	}
	



	fb[index].dirty = 0;
	if(count<=10){ //read succeed 
		 if (crc32(0, (unsigned char *)fb[index].env.verify_data, len) != fb[index].env.verify_crc) {
				 printf("commit:count<=10  crc error  write again\n");
				//write crc to flash
				to = fb[index].flash_offset;
				len = sizeof(fb[index].env.crc);
				//flash_write((char *)&fb[index].env.crc, to, len);

				count=0;
				while(flash_write((char *)&fb[index].env.crc, to, len)<0){
					printf("write crc to flash error count=%d",count);
					if(++count>10){
						fb[index].dirty = 1;
						printf("write crc to flash error,dirty=1");
						break;
					}	
					sleep(1);
				}

				if(count<=10){
					//write data to flash
					to = to + len;
					len = fb[index].flash_max_len - len;
					count=0;
					while(flash_write(fb[index].env.data, to, len)<0){
						printf("write data to flash error count=%d",count);
						if(count++>10){
							fb[index].dirty = 1;
							printf("write data to flash error,dirty=1");
							break;
						}	
						sleep(1);
					}
				}
	
		}	
	}
	FREE(fb[index].env.verify_data); //free it to save momery
	FREE(fb[index].env.data);



printf("debug_nvram_commit:sem unlock,return\n");
//pthread_mutex_unlock(&flash_mutex); //add by zsf 2010-10-28
sem_v(semid);
	return 0;
}

*/




/*
 * clear flash by writing all 1's value
 */
int nvram_clear(int index)
{
	unsigned long to;
	int len;

	LIBNV_PRINT("--> nvram_clear %d\n", index);
	LIBNV_CHECK_INDEX(-1);
	nvram_close(index);

	//construct all 1s env block
	len = fb[index].flash_max_len - sizeof(fb[index].env.crc);
	fb[index].env.data = (char *)malloc(len);
	memset(fb[index].env.data, 0xFF, len);

//pthread_mutex_lock(&flash_mutex); //add by zsf 2010-10-28
        key_t key;
        int semid;

        key=ftok("/",0);
        semid=new_sem(key);
        wait_v(semid);
        sem_p(semid);

	//calculate and write crc
	fb[index].env.crc = (unsigned long)crc32(0, (unsigned char *)fb[index].env.data, len);
	to = fb[index].flash_offset;
	len = sizeof(fb[index].env.crc);
	flash_write((char *)&fb[index].env.crc, to, len);

	//write all 1s data to flash
	to = to + len;
	len = fb[index].flash_max_len - len;
	flash_write(fb[index].env.data, to, len);
	FREE(fb[index].env.data);
	LIBNV_PRINT("clear flash from 0x%x for 0x%x bytes\n", (unsigned int *)to, len);
	fb[index].dirty = 0;

//pthread_mutex_unlock(&flash_mutex); //add by zsf 2010-10-28
sem_v(semid);
	return 0;
}

#if 0
//WARNING: this fuunction is dangerous because it erases all other data in the same sector
int nvram_erase(int index)
{
	int s, e;

	LIBNV_PRINT("--> nvram_erase %d\n", index);
	LIBNV_CHECK_INDEX(-1);
	nvram_close(index);

	s = fb[index].flash_offset;
	e = fb[index].flash_offset + fb[index].flash_max_len - 1;
	LIBNV_PRINT("erase flash from 0x%x to 0x%x\n", s, e);
	FlashErase(s, e);
	return 0;
}
#endif

int getNvramNum(void)
{
	return FLASH_BLOCK_NUM;
}

unsigned int getNvramOffset(int index)
{
	LIBNV_CHECK_INDEX(0);
	return fb[index].flash_offset;
}

char *getNvramName(int index)
{
	LIBNV_CHECK_INDEX(NULL);
	return fb[index].name;
}

unsigned int getNvramBlockSize(int index)
{
	LIBNV_CHECK_INDEX(0);
	return fb[index].flash_max_len;
}

unsigned int getNvramIndex(char *name)
{
	int i;
	for (i = 0; i < FLASH_BLOCK_NUM; i++) {
		if (!strcmp(fb[i].name, name)) {
			return i;
		}
	}
	return -1;
}

void toggleNvramDebug()
{
	if (libnvram_debug) {
		libnvram_debug = 0;
		printf("%s: turn off debugging\n", __FILE__);
	}
	else {
		libnvram_debug = 1;
		printf("%s: turn ON debugging\n", __FILE__);
	}
}


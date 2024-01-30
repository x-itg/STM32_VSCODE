#ifndef __GEN_LIST_H__
#define __GEN_LIST_H__
#include "sys.h"
#include "glb_def.h"


extern	MSG_LIST recvlist;
extern  MSG_LIST sendlist;
	
	
#define list_get_next(h,p)  (((DBL_LNK *)p)->next == (DBL_LNK *)h ? NULL : ((DBL_LNK *)p)->next) 
#define LIST_UNLINK(h,n)  do{				\
		list_unlink((DBL_LNK **)h,(DBL_LNK *)n);                                         	\
        } while (0)
#define LIST_ADD_FIRST(h,n)  do{								\
		list_add_last((DBL_LNK**)(&(h)->list_head),(DBL_LNK*)n);      \
        } while (0)
#define LIST_ADD_LAST(h,n)  do{								\
		list_add_last((DBL_LNK**)(&(h)->list_head),(DBL_LNK*)n);      \
        } while (0)
#define LIST_ADD_LAST_CON(h,n)  do{							\
		list_add_last((DBL_LNK**)(&((h)->list_head)),(DBL_LNK*)n);      \
        } while (0)

ST_VOID *list_get_first (DBL_LNK **);		

ST_RET	list_unlink         (DBL_LNK **, DBL_LNK *);
ST_RET	list_add_first      (DBL_LNK **, DBL_LNK *);
ST_RET	list_add_last       (DBL_LNK **, DBL_LNK *);	
ST_RET 	list_move_to_first  (DBL_LNK **, DBL_LNK **, DBL_LNK *);
ST_RET 	list_find_node      (DBL_LNK *,  DBL_LNK *);
ST_RET 	list_add_node_after (DBL_LNK *,  DBL_LNK *);
ST_INT  list_get_sizeof     (DBL_LNK *);
ST_VOID *list_get_last       (DBL_LNK **);


ST_VOID *list_find_prev (DBL_LNK *list_head_ptr, DBL_LNK *cur_node);
ST_VOID *list_find_last (DBL_LNK *list_head_ptr);

ST_VOID* find_node_id(DBL_LNK* list_head, ST_UINT id,ST_UCHAR flag);
ST_VOID* find_node_dev(DBL_LNK* list_head, ST_CHAR* dev_num);
ST_VOID* find_node_arq(DBL_LNK* list_head, ST_UCHAR msg_id,ST_INT fd);

ST_VOID list_print_links (DBL_LNK *list_head_ptr);


#endif

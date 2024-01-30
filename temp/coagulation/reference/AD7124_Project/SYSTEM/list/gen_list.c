#include "gen_list.h"

/***************************************************************************
函数名:	list_unlink
返回值: signed int
参数:  DBL_LNK **list_head_ptr: 链表头指针地址
	   DBL_LNK *node_ptr:目标节点指针
函数功能: 将目标节点从链表中移出
***************************************************************************/
ST_RET list_unlink (DBL_LNK **list_head_ptr, DBL_LNK *node_ptr)
{
	if(list_find_node(*list_head_ptr,node_ptr) == 0){
		if ((node_ptr->next == *list_head_ptr) && (node_ptr == *list_head_ptr)) {
			*list_head_ptr = NULL;
			return (SD_SUCCESS);
	    }else {
			if (node_ptr == *list_head_ptr)	
				*list_head_ptr = node_ptr->next;		
			(node_ptr->next)->prev = node_ptr->prev;	
			(node_ptr  ->prev)->next = node_ptr->next;		
	    }
		return (SD_SUCCESS);
	}
	return(SD_FAILURE);
}
   
/***************************************************************************
函数名:	list_add_first
返回值: signed int
参数:  DBL_LNK **list_head_ptr: 链表头指针地址
	   DBL_LNK *node_ptr:目标节点指针
函数功能:往表头添加节点
***************************************************************************/
ST_RET list_add_first (DBL_LNK **list_head_ptr, DBL_LNK *node_ptr)
{
	DBL_LNK *list_tail_ptr;
	if (*list_head_ptr == NULL) {
		node_ptr->next = node_ptr;
		node_ptr->prev = node_ptr;
    }else {
		list_tail_ptr = (*list_head_ptr)->prev;
		node_ptr->next = *list_head_ptr;
		node_ptr->prev = list_tail_ptr;
		list_tail_ptr->next = node_ptr;
		(*list_head_ptr)->prev = node_ptr;
    }
	*list_head_ptr = node_ptr;		
	return (SD_SUCCESS);
}

/***************************************************************************
函数名:	list_add_last
返回值: signed int
参数:  DBL_LNK **list_head_ptr: 链表头指针地址
	   DBL_LNK *node_ptr:目标节点指针
函数功能: 往表未尾添加节点
***************************************************************************/
ST_RET list_add_last (DBL_LNK **list_head_ptr, DBL_LNK *node_ptr)
{
	DBL_LNK *list_tail_ptr;

	if (*list_head_ptr == NULL) {
		node_ptr->next = node_ptr;
		node_ptr->prev = node_ptr;
		*list_head_ptr = node_ptr;
    }else{
		list_tail_ptr = (*list_head_ptr)->prev;
		list_tail_ptr->next = node_ptr;
		node_ptr->prev = list_tail_ptr;
		node_ptr->next = *list_head_ptr;
		(*list_head_ptr)->prev = node_ptr;
    }
	return (SD_SUCCESS);
}
/***************************************************************************
函数名:	list_get_first
返回值: void *
参数:  DBL_LNK **list_head_ptr: 链表头指针地址
函数功能: 取表头节点，并移除
***************************************************************************/
ST_VOID *list_get_first (DBL_LNK **list_head_ptr)
{
	DBL_LNK *node_ptr;

  	if (*list_head_ptr == NULL)
    	return (NULL);

  	node_ptr = *list_head_ptr;
  	list_unlink (list_head_ptr, node_ptr);
  	return (node_ptr);
}

/***************************************************************************
函数名:	list_get_last
返回值: ST_VOID*    
参数:  DBL_LNK **list_head_ptr:  链表头指针地址
函数功能:  取表末尾节点，并移除
***************************************************************************/
ST_VOID *list_get_last (DBL_LNK **list_head_ptr)
{
	DBL_LNK *node_ptr;
	if (*list_head_ptr == NULL)
		return (NULL);
	node_ptr = *list_head_ptr;
	node_ptr = node_ptr->prev;
	list_unlink (list_head_ptr, node_ptr);
	return (node_ptr);
}

/***************************************************************************
函数名:	list_add_last
返回值: signed int
参数:  DBL_LNK **src_list_head_ptr: 源链表头指针地址
		DBL_LNK **src_list_head_ptr: 目标链表头指针地址
	   DBL_LNK *node_ptr:目标节点指针
函数功能:将目标节点从源链表中移除，并添加到目标链表表头
***************************************************************************/
ST_RET list_move_to_first (DBL_LNK **src_list_head_ptr, 
                          DBL_LNK **dest_list_head_ptr, 
			  DBL_LNK *node_ptr)
{
	ST_RET ret_code;   
	if (!(ret_code = list_unlink (src_list_head_ptr, node_ptr))){
		list_add_first (dest_list_head_ptr, node_ptr);
    }
	return (ret_code);
}

/***************************************************************************
函数名:	list_find_node
返回值: signed int
参数:  DBL_LNK *list_head_ptr: 链表头指针地址
	   DBL_LNK *node_ptr:目标节点指针
函数功能:在链表中查找目标节点
***************************************************************************/
ST_RET list_find_node (DBL_LNK *list_head_ptr, DBL_LNK *node_ptr)
{
	DBL_LNK	  *temp_ptr;
	DBL_LNK	  *list_tail_ptr;
	ST_RET	ret_code;
     
	if ((list_head_ptr == NULL) || (node_ptr == NULL)){
		ret_code = SD_FAILURE;
	}else {
		temp_ptr = list_head_ptr;
		list_tail_ptr = list_head_ptr->prev;

		while ((temp_ptr != list_tail_ptr) && (temp_ptr != node_ptr)){
			temp_ptr = temp_ptr->next;
		}
		if (temp_ptr == node_ptr){
			ret_code = SD_SUCCESS;
		}else{
			ret_code = SD_FAILURE;
		}
    }
	return (ret_code);
}


/************************************************************************/
/*		           list_print_links				*/
/************************************************************************/

#if 0

ST_VOID list_print_links (DBL_LNK *list_head_ptr)
{
	DBL_LNK	*temp_ptr;
	DBL_LNK	*list_tail_ptr;
	ST_INT i;

  i = 1;
  if (list_head_ptr == NULL)
    {
    printf ("\nThe list is empty");
    }
  else
    {
    temp_ptr = list_head_ptr;
    list_tail_ptr = list_head_ptr->prev;
    printf ("\npointer to the head of the list is: %lx",list_head_ptr);
    printf ("\n");
    printf ("\npointer to node number %d is: %lx",i, temp_ptr);
    printf ("\nnode number:  next ptr is: %lx",temp_ptr->next);
    printf ("\n              prev ptr is: %lx",temp_ptr->prev);
    while ((temp_ptr != list_tail_ptr))
      {
      temp_ptr = temp_ptr->next;
      i++;
      printf ("\npointer to node number %d is: %lx",i, temp_ptr);
      printf ("\nnode number:  next ptr is: %lx",temp_ptr->next);
      printf ("\n              prev ptr is: %lx",temp_ptr->prev);
      }
    }
  }
#endif

/***************************************************************************
函数名:	list_find_node
返回值: signed int
参数:  DBL_LNK *cur_node: 源节点(现有节点)
	   DBL_LNK *new_node:目标节点(新节点)
函数功能:将目标节点添加到源节点后
***************************************************************************/
ST_RET list_add_node_after (DBL_LNK *cur_node, DBL_LNK *new_node)
{
	DBL_LNK	*next_node;
	ST_RET	ret_val;

	ret_val = SD_FAILURE;
	if (new_node == NULL){
		;
    }else if (cur_node == NULL){
		;
    }else{
		next_node = cur_node->next;
		new_node->next = next_node;
		new_node->prev = cur_node;
		next_node->prev = new_node;
		cur_node->next = new_node;
		ret_val = SD_SUCCESS;
    }
	return (ret_val);
}    


/***************************************************************************
函数名:	list_get_sizeof
返回值: signed int
参数:  DBL_LNK *list_head_ptr: 链表表头指针
函数功能:返回链表中节点数
***************************************************************************/
ST_INT list_get_sizeof (DBL_LNK *list_head_ptr)
{
	ST_INT count;
	DBL_LNK *cur_node;

	count = 0;
	cur_node = list_head_ptr;

	while (cur_node != NULL) {
		count ++;
		cur_node = (DBL_LNK *) list_get_next (list_head_ptr, cur_node);
    }

	return (count);
}

/***************************************************************************
函数名:	list_find_prev
返回值: ST_VOID*  
参数:  DBL_LNK *list_head_ptr: 链表表头指针
	   DBL_LNK *cur_node:基节点
函数功能: 查找并返回基节点前的节点
***************************************************************************/
ST_VOID *list_find_prev (DBL_LNK *list_head_ptr, DBL_LNK *cur_node)
{
	DBL_LNK *prev_node = NULL;

	if (list_head_ptr == NULL){
		;
    }else if (cur_node == NULL){
		;
    }else if (cur_node != list_head_ptr){
		prev_node = cur_node->prev;
    }
	return (prev_node);
}

/***************************************************************************
函数名:	list_find_prev
返回值: ST_VOID*  
参数:  DBL_LNK *list_head_ptr:  链表表头指针
函数功能: 返回链表最后一个节点
***************************************************************************/
ST_VOID *list_find_last (DBL_LNK *list_head_ptr)
{
	DBL_LNK *last_node;

	if (list_head_ptr){
		last_node = list_head_ptr->prev;
	}else{	
		last_node = NULL;
    }

	return (last_node);
}
/***************************************************************************
函数名:	list_find_prev
返回值: ST_VOID*  
参数:  DBL_LNK *list_head_ptr:  链表表头指针
		id:节点中的一个内容can_id
函数功能: 按内容查找节点
***************************************************************************/
ST_VOID* find_node_id(DBL_LNK* list_head, ST_UINT id,ST_UCHAR flag){
	DBL_LNK* ptr = list_head;
	LINKER_NODE* node;
	while (ptr != NULL) {
		node = (LINKER_NODE*)ptr;
		switch(flag){
//			case CAN_ID:
//				if(node->can_id == id)
//					return node;		
//				break;
//			case FD_ID:
//				if(node->fd == id)
//					return ptr;		
//				break;
//			case MSG_ID:
//				if(node->msg_id == id)
//					return ptr;
//				break;
//			case BOSS:
//				if(node->isBoss == 1)
//					return node;
//				break;
//			case ADDR:
//				if(node->addr == id)
//					return node;
				break;
			default:
				break;
		}
		ptr = (DBL_LNK *) list_get_next (list_head, ptr);
   	}
	return NULL;
}
/***************************************************************************
函数名:	list_find_prev
返回值: ST_VOID*  
参数:  DBL_LNK *list_head_ptr:  链表表头指针
		msg_id:节点中的一个内容msg__id
函数功能: 按内容查找节点(用于根据msg_id查找重发队列中的元素)
***************************************************************************/
ST_VOID* find_node_arq(DBL_LNK* list_head, ST_UCHAR msg_id,ST_INT fd){
	DBL_LNK* ptr = list_head;
	MSG_NODE* node;
	if(ptr == NULL){
//		syd_logger_debug(&g_log_file,"SYS,find_node_arq,NULL\n");;
	}else {
		do{
			node = (MSG_NODE*)ptr;
			if((node->msg_id == msg_id)&&(node->fd == fd)){
				return ptr;
			}else{
				ptr = ptr->next;
			}
		}while(ptr!=list_head);
	}
	return ptr;
}
/***************************************************************************
函数名:	list_find_prev
返回值: ST_VOID*  
参数:  DBL_LNK *list_head_ptr:  链表表头指针
		dev_num:节点中的一个内容(设备编号)
函数功能: 按内容查找节点
***************************************************************************/
ST_VOID* find_node_dev(DBL_LNK* list_head, ST_CHAR *dev_num){
	DBL_LNK* ptr = list_head;
	LINKER_NODE* node;
	if(list_head == NULL){
		return NULL;
	}else {
		do{
			node = (LINKER_NODE*)ptr;
			if(str_cmp(node->dev_data.dev_number,dev_num,NUMBER_LEN)==0)
				return ptr;
			else
				ptr = ptr->next;
		}while(ptr!=list_head);
	}
	return NULL;
}


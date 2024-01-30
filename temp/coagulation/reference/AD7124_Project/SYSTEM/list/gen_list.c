#include "gen_list.h"

/***************************************************************************
������:	list_unlink
����ֵ: signed int
����:  DBL_LNK **list_head_ptr: ����ͷָ���ַ
	   DBL_LNK *node_ptr:Ŀ��ڵ�ָ��
��������: ��Ŀ��ڵ���������Ƴ�
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
������:	list_add_first
����ֵ: signed int
����:  DBL_LNK **list_head_ptr: ����ͷָ���ַ
	   DBL_LNK *node_ptr:Ŀ��ڵ�ָ��
��������:����ͷ��ӽڵ�
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
������:	list_add_last
����ֵ: signed int
����:  DBL_LNK **list_head_ptr: ����ͷָ���ַ
	   DBL_LNK *node_ptr:Ŀ��ڵ�ָ��
��������: ����δβ��ӽڵ�
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
������:	list_get_first
����ֵ: void *
����:  DBL_LNK **list_head_ptr: ����ͷָ���ַ
��������: ȡ��ͷ�ڵ㣬���Ƴ�
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
������:	list_get_last
����ֵ: ST_VOID*    
����:  DBL_LNK **list_head_ptr:  ����ͷָ���ַ
��������:  ȡ��ĩβ�ڵ㣬���Ƴ�
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
������:	list_add_last
����ֵ: signed int
����:  DBL_LNK **src_list_head_ptr: Դ����ͷָ���ַ
		DBL_LNK **src_list_head_ptr: Ŀ������ͷָ���ַ
	   DBL_LNK *node_ptr:Ŀ��ڵ�ָ��
��������:��Ŀ��ڵ��Դ�������Ƴ�������ӵ�Ŀ�������ͷ
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
������:	list_find_node
����ֵ: signed int
����:  DBL_LNK *list_head_ptr: ����ͷָ���ַ
	   DBL_LNK *node_ptr:Ŀ��ڵ�ָ��
��������:�������в���Ŀ��ڵ�
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
������:	list_find_node
����ֵ: signed int
����:  DBL_LNK *cur_node: Դ�ڵ�(���нڵ�)
	   DBL_LNK *new_node:Ŀ��ڵ�(�½ڵ�)
��������:��Ŀ��ڵ���ӵ�Դ�ڵ��
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
������:	list_get_sizeof
����ֵ: signed int
����:  DBL_LNK *list_head_ptr: �����ͷָ��
��������:���������нڵ���
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
������:	list_find_prev
����ֵ: ST_VOID*  
����:  DBL_LNK *list_head_ptr: �����ͷָ��
	   DBL_LNK *cur_node:���ڵ�
��������: ���Ҳ����ػ��ڵ�ǰ�Ľڵ�
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
������:	list_find_prev
����ֵ: ST_VOID*  
����:  DBL_LNK *list_head_ptr:  �����ͷָ��
��������: �����������һ���ڵ�
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
������:	list_find_prev
����ֵ: ST_VOID*  
����:  DBL_LNK *list_head_ptr:  �����ͷָ��
		id:�ڵ��е�һ������can_id
��������: �����ݲ��ҽڵ�
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
������:	list_find_prev
����ֵ: ST_VOID*  
����:  DBL_LNK *list_head_ptr:  �����ͷָ��
		msg_id:�ڵ��е�һ������msg__id
��������: �����ݲ��ҽڵ�(���ڸ���msg_id�����ط������е�Ԫ��)
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
������:	list_find_prev
����ֵ: ST_VOID*  
����:  DBL_LNK *list_head_ptr:  �����ͷָ��
		dev_num:�ڵ��е�һ������(�豸���)
��������: �����ݲ��ҽڵ�
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


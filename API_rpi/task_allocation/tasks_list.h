#ifndef tasks_list_h
#define tasks_list_h

#include "task.h"

Items<Task> tasks_list_test(){				// might need to be done with pointers
	Task t1 = Task(1, "take_picture", 1, 1);
	Task t2 = Task(2, "up-stairs", 3, 5);

	Items<Task> task_list;
	task_list.add_item(t1);
	task_list.add_item(t2);

	return task_list;
}


Items<NodeTask> node_task_list_test(){		// might need to be done with pointers

	NodeTask nt1 = NodeTask("a");			// to be used during the navigation
	NodeTask nt2 = NodeTask("b");
	NodeTask nt3 = NodeTask("c",1,1);
	NodeTask nt4 = NodeTask("d");
	NodeTask nt5 = NodeTask("e",1,-1);
	NodeTask nt6 = NodeTask("f",-1,0);

	Items<NodeTask> node_task_list;
	node_task_list.add_item(nt1);
	node_task_list.add_item(nt2);
	node_task_list.add_item(nt3);
	node_task_list.add_item(nt4);
	node_task_list.add_item(nt5);
	node_task_list.add_item(nt6);

	return task_list;
}


#endif
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct mymsgbuf
{
  long mtype;
  struct {
    char  mtext[30];
    int   mnumber;
  } info;
} mybuf;

void send_message(int msqid, int number) {
  mybuf.mtype = 2;
  strcpy(mybuf.info.mtext, "Hello from program B!");
  mybuf.info.mnumber = number;
  //
  // Send the message. If there is an error,
  // report it and delete the message queue from the system.
  //
  if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf.info), 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }
}

void receive_message(int msqid) {
  if (msgrcv(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf.info), 1, 0) < 0) {
    printf("Can\'t receive message from queue\n");
    exit(-1);
  }
}

int main(void)
{
  int msqid;
  char pathname[]="task2a.c";
  key_t  key;
  int len;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  int multiplier_b = 3;
  int current_value = 0;

  while (1) {
    receive_message(msqid);

    printf("Program B: type = %ld, text = %s, value = %d\n", mybuf.mtype, mybuf.info.mtext, mybuf.info.mnumber);

    //
    // If the received message mnumber is 0,
    // then terminate and remove the message queue from the system.
    //
    if (mybuf.info.mnumber == 0) {
      msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
      exit(0);
    }

    current_value = mybuf.info.mnumber * multiplier_b;

    send_message(msqid, current_value);
  }

  return 0;
}

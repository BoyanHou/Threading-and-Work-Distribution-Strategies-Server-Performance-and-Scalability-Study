#ifndef GLB_SET
#define GLB_SET

#define ADDRINFO_HINT_IP "0.0.0.0"
#define ADDRINFO_HINT_PORT "9999"
#define HTTP_PORT "80"
#define SERVER_BUF_SIZE 1024
#define CLIENT_BUF_SIZE 1024
#define PORT_STR_LEN 10          // port string max length
#define BACK_LOG 10              // socket listen back log
#define IP_LEN INET6_ADDRSTRLEN  // IP max len
#define PRE_CREATE_THREADS 0     // pre-create threads mode
#define PER_CREATE_THREADS 1     // per-create threads mode
#define SERVER_PORT "12345"
#define SERVER_IP "0.0.0.0"
#define PRE_THREAD_NUM 1000  // number of pre-created thread num (int SERVER)
#define CLIENT_BUCKET_NUM_OFF_RANGE \
  1.0  // when client is randomly generating bucket index, it can have a range either more than the server has(>1.0) or has fewer(<1.0)
#define SERVER_RUNTIME 300  // the server runtime (in second)
#endif

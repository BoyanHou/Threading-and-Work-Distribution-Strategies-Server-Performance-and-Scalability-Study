## ERSS HW4 README

Team member: Boyan Hou(bh214), Kefan Lin(kl352)

---

### 1. How to change the parameters

- **To change the server's running time**:

  ```shell
  $ emacs ./global_settings.h
  ```

  **Then find:(in line 19)**

  ```c
  #define SERVER_RUNTIME 300 // the server runtime (in second)
  ```

  **Change 300 to any number you want (in second)**.

  ---

- **To change threading strategies**:

  ```shell
  $ cd Server/
  $ emacs server_main.cpp
  ```

  **Then find:(in line 23)**

  ```c
  Server server(PER_CREATE_THREADS, bucket_num, SERVER_IP, SERVER_PORT);
  ```

  **Change the first argument to either "*PER_CREATE_THREADS*" or "*PRE_CREATE_THREADS*".**

  - *"PER_CREATE_THREADS" : The server create threads per-request.*

  - *"PRE_CREATE_THREADS" : The server pre-create threads.*

  ---

- **To change pre-create thread number**

  ```shell
  $ emacs ./global_settings.h
  ```

  **Then find:(in line 16)**

  ```c
  #define PRE_THREAD_NUM 1000  // number of pre-created thread num (int SERVER)
  ```

  **Change 1000 to any number you want in range [1,1024]**

  ---

- **To change CPU cores**

  ```shell
  $ emacs docker-compose.yml 
  ```

  **Then find: (in line 7)**

  ```
  cpuset: '0,1,2,3'
  ```

  **Change '0,1,2,3' to any CPU core combination you want. (e.g. '0,1' means run the server on core 0 and core 1)**

---

- **To change bucket size**

  ```shell
  $ emacs Dockerfile
  ```

  **Then find: (in line 14)**

  ```dockerfile
  CMD ["2048"]
  ```

  **Change "2048" to any bucket size you want. (e.g. 32, 128, 512, 2048)** 

---

### 2. How to run the whole system

- **Run the server**

  - In the outermost directory, run:

    ```shell
    $ sudo docker-compose up --build
    ```

  - If you need to modify some arguments, read **Part1: How to change the parameters**.

- **Run the client**

  - In the **testing/** directory, run:

  ```shell
  $ ./<client> <server_ip> <server_port> <thread_num> <bucket_num> <delay_min> <delay_max>
  ```

  - for the \<client\> part, fill in:
    - test_func: for functionality test; notice: due to the print in the process, this might be not as fast as test_scale
    - test_scale: for scalability test, this will not print any thing, and is the test we used for our scalability test.
  - <server_ip>: The IP address of the server
- <server_port>: The port that the server program binds to
  - <thread_num>: Number of threads that the client creates to send requests
- <bucket_num>: Number of buckets that the server creates(this must be the same as the server side)
  

  
  
  
  


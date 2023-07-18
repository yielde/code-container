/*
  成功返回0
  失败返回-1, 设置errno
*/
int atomic_log_array(char *s, int len);  // 已知字节数的数组(写入缓冲区)
int atomic_log_printf(char *fmt, ...);  // fprintf写入方式(写入缓冲区)
int atomic_log_string(char *s);         // 记录字符串(写入缓冲区)
int atomic_log_clear();         // 释放缓冲区，不输出缓冲区内容
int atomic_log_close();         // 关闭日志文件
int atomic_log_open(char *fn);  // 创建日志文件
int atomic_log_send();          // 输出缓存区内容，释放缓冲区
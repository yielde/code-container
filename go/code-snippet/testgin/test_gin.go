/*
 * @Author: Galen Tong
 * @Date: 2022-12-19 17:15:55
 * @LastEditTime: 2022-12-20 14:29:57
 * @Description:
 */
package main

import (
	"github.com/gin-gonic/gin"
)


func MyHandler(c *gin.Context){
	c.JSON(200, gin.H{
		"hello": "hello world",
	})
}

func Login(c *gin.Context) {
	c.HTML(200, "login.html", nil)
}

func DoLogin(c *gin.Context) {
	username := c.PostForm("username")
	password := c.PostForm("password")
	c.HTML(200, "index.html", gin.H{
		"username":username,
		"password":password,
	})
}

// func main(){
// 	e:=gin.Default()
// 	e.LoadHTMLGlob("templates/*")
// 	e.GET("/login", Login)
// 	e.POST("/login", DoLogin)
// 	e.Run()
// }

// func main() {
// 	r := gin.Default()
// 	r.GET("/ping", func(c *gin.Context) {
// 		c.JSON(200, gin.H{
// 			"message":"pong",
// 			"age":20,
// 		})
// 	})
// 	r.Run()
// }

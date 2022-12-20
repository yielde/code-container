/*
 * @Author: Galen Tong
 * @Date: 2022-12-20 14:30:13
 * @LastEditTime: 2022-12-20 15:41:54
 * @Description:
 */
package main

import (

	"github.com/gin-gonic/gin"
)

var secrets = gin.H{
	"foo": gin.H{"email": "foo@email.com", "phone": "123"},
	"bar": gin.H{"email": "bar@email.com", "phone": "123"},
}

// func main() {
// 	serve := gin.Default()
// 	authorized := serve.Group("/admin", gin.BasicAuth(gin.Accounts{
// 		"foo": "foo",
// 		"bar": "123",
// 	}))

// 	authorized.GET("/secrets", func(c *gin.Context) {
// 		user := c.MustGet(gin.AuthUserKey).(string)
// 		fmt.Println(user)
// 		if secret, ok := secrets[user]; ok {
// 			c.JSON(http.StatusOK, gin.H{"user": user, "secret": secret})
// 		} else {
// 			c.JSON(http.StatusOK, gin.H{"user": user, "secret": "No SECRET"})
// 		}
// 	})
// 	serve.Run()
// }

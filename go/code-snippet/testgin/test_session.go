/*
 * @Author: Galen Tong
 * @Date: 2022-12-20 15:30:04
 * @LastEditTime: 2022-12-20 15:43:09
 * @Description:
 */
package main

import (
	"github.com/gin-contrib/sessions"
	"github.com/gin-contrib/sessions/cookie"
	"github.com/gin-gonic/gin"
	"net/http"
)

func main() {
	g := gin.Default()
	store := cookie.NewStore([]byte("secret"))
	g.Use(sessions.Sessions("mysession", store))

	g.GET("/hello", func(ctx *gin.Context) {
		session := sessions.Default(ctx)

		if session.Get("hello") != "world" {
			session.Set("hello", "world")
			session.Save()
		}

		ctx.JSON(http.StatusOK, session.Get("hello"))
	})
	g.Run()
}

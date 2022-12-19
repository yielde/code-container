/*
 * @Author: Galen Tong
 * @Date: 2022-12-19 16:52:33
 * @LastEditTime: 2022-12-19 17:15:46
 * @Description:
 */
package main

import (
	"fmt"
	"log"
	"net/http"

	"github.com/julienschmidt/httprouter"
)

func Index(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	fmt.Fprintf(w, "Welcom!\n")
}

func Hello(w http.ResponseWriter, r *http.Request, ps httprouter.Params) {
	fmt.Fprintf(w, "hello, %s\n", ps.ByName("name"))
}


// func main(){
// 	router := httprouter.New()
// 	router.GET("/", Index)
// 	router.GET("/hello/:name", Hello)
// 	log.Fatal(http.ListenAndServe(":8080", router))
// }
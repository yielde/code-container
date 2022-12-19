/*
 * @Author: Galen Tong
 * @Date: 2022-12-19 12:12:35
 * @LastEditTime: 2022-12-19 18:31:39
 * @Description:
 */
package main

import (
	"html/template"
	"net/http"
	"os"
)

func test1() {
	name := "Galen Tong"
	temp := "hello, {{.}}"
	tmpl, err := template.New("test").Parse(temp)
	if err != nil {
		panic(err)
	}
	err = tmpl.Execute(os.Stdout, name)
	if err != nil {
		panic(err)
	}
}

// struct
type Person struct {
	Name string
	Age  int
}

func test2() {
	gl := Person{"Galen", 20}
	temp := "hello, {{.Name}}, Your aget {{.Age}}"
	tmpl, err := template.New("test").Parse(temp)
	if err != nil {
		panic(err)
	}
	err = tmpl.Execute(os.Stdout, gl)
	if err != nil {
		panic(err)
	}
}

//

// test.html template
func tmpl(w http.ResponseWriter, r *http.Request) {
	t1, err := template.ParseFiles("test.html")
	if err != nil {
		panic(err)
	}
	t1.Execute(w, "hello world")
}

//

// test2.html template
func tmpl2(w http.ResponseWriter, r *http.Request) {
	t1, err := template.ParseFiles("test2.html")
	if err != nil {
		panic(err)
	}
	t1.Execute(w, nil)
}

//

// test3.html template
func tmpl3(w http.ResponseWriter, r *http.Request) {
	t1, err := template.ParseFiles("test3.html")
	if err != nil {
		panic(err)
	}
	s := []string{"galen", "tong", "age 20"}
	t1.Execute(w, s)
}

//

// test3.html template
type HTML struct {
	Head string
	Foot string
}

func tmpl4(w http.ResponseWriter, r *http.Request) {
	t1, err := template.ParseFiles("index.html", "header.html", "footer.html")
	if err != nil {
		panic(err)
	}
	html := HTML{"header", "footer"}
	t1.Execute(w, html)
}

//

// func main() {
// 	// test1()
// 	// test2()
// 	server := http.Server{
// 		Addr: "0.0.0.0:9988",
// 	}
// 	http.HandleFunc("/tmpl", tmpl4)
// 	server.ListenAndServe()
// }

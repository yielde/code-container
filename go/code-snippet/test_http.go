/*
 * @Author: Galen Tong
 * @Date: 2022-12-18 21:35:10
 * @LastEditTime: 2022-12-19 14:27:31
 * @Description:
 */
package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"net/url"
	"strings"
	"sync"
	"time"
)

func testGet() {
	url := "http://apis.juhe.cn/simpleWeather/query?key=087d7d10f700d20e27bb753cd806e40b&city=北京"
	r, err := http.Get(url)
	if err != nil {
		log.Fatal(err)
	}
	defer r.Body.Close()
	b, _ := ioutil.ReadAll(r.Body)
	fmt.Printf("b: %v\n", string(b))
}

func testGet2() {
	params := url.Values{}
	Url, err := url.Parse("http://apis.juhe.cn/simpleWeather/query")
	if err != nil {
		return
	}
	params.Set("key", "087d7d10f700d20e27bb753cd806e40b")
	params.Set("city", "北京")

	Url.RawQuery = params.Encode()
	urlPath := Url.String()
	fmt.Println(urlPath)
	resp, err := http.Get(urlPath)
	if err != nil {
		log.Fatal(err)
	}
	defer resp.Body.Close()
	body, _ := ioutil.ReadAll(resp.Body)
	fmt.Println(string(body))
}

func testParseJson() {
	type result struct {
		Args    string            `json:"args"`
		Headers map[string]string `json:"headers"`
		Origin  string            `json:"origin"`
		Url     string            `json:"url"`
	}

	resp, err := http.Get("http://httpbin.org/get")
	if err != nil {
		return
	}
	defer resp.Body.Close()
	body, _ := ioutil.ReadAll(resp.Body)
	fmt.Println(string(body))
	var res result
	_ = json.Unmarshal(body, &res)
	fmt.Printf("%#v", res)
}

func testAddHeader() {
	client := &http.Client{}
	req, _ := http.NewRequest("GET", "http://httpbin.org/get", nil)
	req.Header.Add("name", "Galen")
	req.Header.Add("age", "80")
	resp, err := client.Do(req)
	if err != nil {
		return
	}
	defer resp.Body.Close()
	body, _ := ioutil.ReadAll(resp.Body)
	fmt.Println(string(body))
}

func testPost() {
	path := "http://apis.juhe.cn/simpleWeather/query"
	urlValues := url.Values{}
	urlValues.Set("key", "087d7d10f700d20e27bb753cd806e40b")
	urlValues.Set("city", "北京")
	r, err := http.PostForm(path, urlValues)
	if err != nil {
		log.Fatal(err)
	}
	defer r.Body.Close()
	b, _ := ioutil.ReadAll(r.Body)
	fmt.Printf("%v\n", string(b))
}

func testPost2() {
	urlValues := url.Values{
		"name": {"Galen"},
		"age":  {"18"},
	}
	reqBody := urlValues.Encode()
	resp, _ := http.Post("http://httpbin.org/post", "text/html", strings.NewReader(reqBody))
	body, _ := ioutil.ReadAll(resp.Body)
	fmt.Println(string(body))
}

func testPostJson() {
	data := make(map[string]interface{})
	data["site"] = "www.galen.com"
	data["name"] = "Galen"
	bytesData, _ := json.Marshal(data)
	resp, _ := http.Post("http://httpbin.org/post", "application/json", bytes.NewReader(bytesData))
	body, _ := ioutil.ReadAll(resp.Body)
	fmt.Println(string(body))
}

func testClient() {
	client := http.Client{
		Timeout: time.Second * 5,
	}
	url := "http://apis.juhe.cn/simpleWeather/query?key=087d7d10f700d20e27bb753cd806e40b&city=北京"
	req, err := http.NewRequest(http.MethodGet, url, nil)
	if err != nil {
		log.Fatal(err)
	}
	req.Header.Add("referer", "http://apis.juhe.cn/")
	res, err2 := client.Do(req)
	if err2 != nil {
		log.Fatal(err2)
	}
	defer res.Body.Close()
	b, _ := ioutil.ReadAll(res.Body)
	fmt.Printf("b: %v\n", string(b))
}

func testHttpServer() {
	f := func(resp http.ResponseWriter, req *http.Request) {
		io.WriteString(resp, "hello world")
	}
	http.HandleFunc("/hello", f)
	err := http.ListenAndServe(":9933", nil)
	if err != nil {
		log.Fatal(err)
	}
}

// concurrent processing
type countHandler struct {
	mu sync.Mutex
	n  int
}

func (h *countHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	h.mu.Lock()
	defer h.mu.Unlock()
	h.n++
	fmt.Fprintf(w, "count is %d\n", h.n)
}

func testHttpServer2() {
	http.Handle("/count", new(countHandler))
	log.Fatal(http.ListenAndServe(":8080", nil))
}

// concurrent processing

// func main() {
// 	testGet()
// 	testGet2()
// 	testParseJson()
// 	testAddHeader()
// 	testPost()
// 	testPost2()
// 	testPostJson()
// 	testClient()
// 	testHttpServer()
// 	testHttpServer2()
// }

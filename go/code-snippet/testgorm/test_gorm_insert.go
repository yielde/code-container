/*
 * @Author: Galen Tong
 * @Date: 2022-12-20 22:10:20
 * @LastEditTime: 2022-12-20 22:23:00
 * @Description:
 */
package main

import (
	"time"

	"gorm.io/driver/mysql"
	"gorm.io/gorm"
)

var db *gorm.DB

func createDB() {
	dsn := "root:p@ssw0rd@tcp(127.0.0.1:3306)/golang_db?charset=utf8mb4&parseTime=True&loc=Local"
	b, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
	if err != nil {
		panic("failed to connect database")
	}
	db = b
}

func init() {
	createDB()
}

type User struct {
	gorm.Model
	Name      string
	Age       int
	Birsthday time.Time
}

func createTable() {
	db.AutoMigrate(User{})
}

func insert() {
	user := User{
		Name: "tom",
		Age: 20,
		Birsthday: time.Now(),
	}
	db.Create(user)
}

func main() {
	// createTable()

}

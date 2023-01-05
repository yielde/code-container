/*
 * @Author: Galen Tong
 * @Date: 2022-12-20 22:10:20
 * @LastEditTime: 2022-12-21 11:42:41
 * @Description:
 */
package main

import (
	"fmt"
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
		Name:      "tom",
		Age:       20,
		Birsthday: time.Now(),
	}
	result := db.Create(&user)
	fmt.Printf("result.RowsAffected: %v\n", result.RowsAffected)
	fmt.Printf("user.ID: %v\n", user.ID)
}

func batchInsert() {
	var users = []User{{Name: "Galen"}, {Name: "Tong"}, {Name: "galen Tong"}}
	db.Create(&users)
}

func mapInsert() {
	db.Model(&User{}).Create(map[string]interface{}{
		"Name": "mapuser", "Age": 10,
	})
}

func (u *User) BeforeCreate(tx *gorm.DB) (err error) {
	fmt.Println("beforeCreate...")
	return
}



func main() {
	// createTable()
	// insert()
	// batchInsert()
	mapInsert()

}

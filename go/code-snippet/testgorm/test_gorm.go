/*
 * @Author: Galen Tong
 * @Date: 2022-12-20 16:59:27
 * @LastEditTime: 2022-12-21 11:20:56
 * @Description:
 */
package main

// import (
// 	"fmt"

	// "gorm.io/driver/mysql"
// 	"gorm.io/gorm"
// )

// type Product struct {
// 	gorm.Model
// 	Code  string
// 	Price uint
// }

// func create(db *gorm.DB) {
// 	db.AutoMigrate(&Product{})

// }

// func insert(db *gorm.DB) {
// 	p := Pruct{
// 		Code:  "1003",
// 		Price: 233,
// 	}
// 	db.Create(&p)
// }

// func find(db *gorm.DB){
// 	var product Product
// 	db.First(&product, 1)
// 	fmt.Printf("%v\n", product)

// 	var prod Product
// 	db.First(&prod, "code = ?", "1003")
// 	fmt.Printf("p: %v\n", prod)
// }

// func update(db *gorm.DB){
// 	var p Product
// 	db.First(&p, 1)
// 	db.Model(&p).Update("Price", 3333)
// 	db.Model(&p).Updates(Product{Price: 2222, Code: "2222"})
// 	db.Model(&p).Updates(map[string]interface{}{"Price": 3333, "Code": "3333"})
// }

// func delete(db *gorm.DB){
// 	var p Product
// 	db.First(&p, 1)
// 	db.Delete(&p)
// }

// func main() {
// 	dsn := "root:p@ssw0rd@tcp(127.0.0.1:3306)/golang_db?charset=utf8mb4&parseTime=True&loc=Local"
// 	db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
// 	if err != nil {
// 		panic("failed to connect database")
// 	}
// 	delete(db)
// }

import axios from 'axios'

const instance = axios.create({
  baseURL: 'https://mock.apifox.cn/m1/2814985-0-default',
  timeout: 10000
})

export const post = (url, data = {}) => {
  return new Promise((resolve, reject) => {
    instance.post(url, data, {
      headers: { 'Content-Type': 'application/json' }
    }).then((response) => {
      resolve(response.data)
    }, (err) => {
      reject(err)
    })
  })
}

export const get = (url, params = {}) => {
  return new Promise((resolve, reject) => {
    instance.get(url, { params }).then((response) => {
      resolve(response.data)
    }, err => {
      reject(err)
    })
  })
}

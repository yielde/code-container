import axios from 'axios'
export const post = (url, data = {}) => {
  return new Promise((resolve, reject) => {
    axios.post(url, data, {
      baseURL: 'https://mock.apifox.cn/m1/2814985-0-default',
      headers: { 'Content-Type': 'application/json' }
    }).then((response) => {
      resolve(response.data)
    }, (err) => {
      reject(err)
    })
  })
}

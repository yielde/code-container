import { createRouter, createWebHashHistory } from 'vue-router'
import HomePage from '../views/home/HomePage'
import LoginPage from '../views/login/LoginPage'
import RegisterPage from '../views/register/RegisterPage'

const routes = [
  {
    path: '/',
    name: 'HomePage',
    component: HomePage
  },
  {
    path: '/login',
    name: 'LoginPage',
    component: LoginPage,
    beforeEnter: (to, from, next) => {
      const isLogin = localStorage.isLogin
      isLogin ? next({ name: 'HomePage' }) : next()
    }
  },
  {
    path: '/register',
    name: 'RegisterPage',
    component: RegisterPage
  }
]

const router = createRouter({
  history: createWebHashHistory(),
  routes
})
// to 要访问还没访问的页面
// from 从哪里访问过来的
// next 访问继续
router.beforeEach((to, from, next) => {
  const isLogin = localStorage.isLogin
  isLogin || to.name === 'LoginPage' || to.name === 'RegisterPage' ? next() : next({ name: 'LoginPage' })
})

export default router

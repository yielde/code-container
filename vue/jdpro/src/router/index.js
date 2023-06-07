import { createRouter, createWebHashHistory } from 'vue-router'
const routes = [
  // 异步路由加载
  {
    path: '/',
    name: 'HomePage',
    component: () => import(/* webpackChunkName: "HomePage" */'../views/home/HomePage')
  },
  {
    path: '/login',
    name: 'LoginPage',
    component: () => import(/* webpackChunkName: "LoginPage" */'../views/login/LoginPage'),
    beforeEnter: (to, from, next) => {
      const isLogin = localStorage.isLogin
      isLogin ? next({ name: 'HomePage' }) : next()
    }
  },
  {
    path: '/register',
    name: 'RegisterPage',
    component: () => import(/* webpackChunkName: "RegisterPage" */'../views/register/RegisterPage')
  },
  {
    path: '/shop/:id',
    name: 'ShopPage',
    component: () => import(/* webpackChunkName: "ShopPage" */'../views/shop/ShopPage')
  },
  {
    path: '/cart',
    name: 'CartPage',
    component: () => import('../views/cart/CartPage')
  },
  {
    path: '/order-confirm/:id',
    name: 'OrderConfirmation',
    component: () => import('../views/order/OrderConfirmation')
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

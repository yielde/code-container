import { createStore } from 'vuex'

export default createStore({
  state: {
    cartList: {}
  },
  getters: {
  },
  mutations: {
    addItemToCart (state, payload) {
      const { shopId, productId, productInfo } = payload
      let shopInfo = state.cartList[shopId]
      if (!shopInfo) { shopInfo = {} }
      let product = shopInfo[productId]
      if (!product) {
        product = productInfo
        product.count = 0
      }
      product.count += 1
      shopInfo[productId] = product
      state.cartList[shopId] = shopInfo
      console.log(state.cartList[shopId][productId].name, state.cartList[shopId][productId].count)
    },
    delCartItem (state, payload) {
      const { shopId, productId } = payload
      const shopInfo = state.cartList[shopId]
      if (!shopInfo) { return 0 }
      const product = shopInfo[productId]
      if (!product || product.count === 0) { return 0 }
      product.count -= 1
      shopInfo[productId] = product
      state.cartList[shopId] = shopInfo
      console.log(state.cartList[shopId][productId].name, state.cartList[shopId][productId].count)
    }
  },
  actions: {
  },
  modules: {
  }
})

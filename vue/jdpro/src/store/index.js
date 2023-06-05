import { createStore } from 'vuex'

const setLocalCartList = (state) => {
  const { cartList } = state
  const cartListString = JSON.stringify(cartList)
  localStorage.cartList = cartListString
}

const getLocalCartList = () => {
  return JSON.parse(localStorage.cartList) || {}
}

export default createStore({
  state: {
    // cartList:{shopId:{shopName:'', productList:{productId:{}}}}
    cartList: getLocalCartList()
  },
  getters: {
  },
  mutations: {
    addItemToCart (state, payload) {
      const { shopId, productId, productInfo } = payload
      const shopInfo = state.cartList[shopId] || { shopName: '', productList: {} }
      let product = shopInfo.productList[productId]
      if (!product) {
        product = productInfo
        product.count = 0
      }
      product.count += 1
      product.check = true
      shopInfo.productList[productId] = product
      state.cartList[shopId] = shopInfo
      setLocalCartList(state)
      console.log(state.cartList[shopId].productList[productId].name, state.cartList[shopId].productList[productId].count)
    },
    delCartItem (state, payload) {
      const { shopId, productId } = payload
      const shopInfo = state.cartList[shopId]?.productList
      if (!shopInfo) { return 0 }
      const product = shopInfo[productId]
      if (!product || product.count === 0) { return 0 }
      product.count -= 1
      shopInfo[productId] = product
      state.cartList[shopId].productList = shopInfo
      setLocalCartList(state)
      console.log(state.cartList[shopId].productList[productId].name, state.cartList[shopId].productList[productId].count)
    },
    changeCartProductItemCheck (state, payload) {
      const productItem = state.cartList[payload.shopId].productList[payload.productId]
      productItem.check = !productItem.check
      setLocalCartList(state)
    },
    cleanCartProducts (state, payload) {
      state.cartList[payload.shopId].productList = {}
      setLocalCartList(state)
    },
    setCartAllChecked (state, payload) {
      const productList = state.cartList[payload.shopId].productList
      for (const i in productList) {
        productList[i].check = true
      }
      setLocalCartList(state)
    },
    addShopName (state, payload) {
      const shopInfo = state.cartList[payload.shopId] || { shopName: '', productList: {} }
      shopInfo.shopName = payload.shopName
      state.cartList[payload.shopId] = shopInfo
      setLocalCartList(state)
    }
  },
  actions: {
  },
  modules: {
  }
})

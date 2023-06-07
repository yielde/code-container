import { useStore } from 'vuex'
import { computed, watchEffect, reactive, ref } from 'vue'
import { get } from '../utils/request'
export const useCommonCartEffect = (shopId) => {
  const store = useStore()
  const cartList = store.state.cartList
  const showCart = ref(false)

  const addItemToCart = (shopId, productId, productInfo, shopName) => {
    addShopName(shopId, shopName)
    store.commit('addItemToCart', { shopId, productId, productInfo })
  }

  const addShopName = (shopId, shopName) => {
    store.commit('addShopName', { shopId, shopName })
  }

  const delCartItem = (shopId, productId) => {
    store.commit('delCartItem', { shopId, productId })
  }

  const productList = computed(() => {
    const productList = cartList[shopId]?.productList || {}
    return productList
  })

  const shopName = computed(() => {
    return cartList[shopId]?.shopName
  })

  const caculations = computed(() => {
    const productList = cartList[shopId]?.productList
    const result = { total: 0, price: 0, allChecked: true }
    if (productList) {
      for (const i in productList) {
        const product = productList[i]
        result.total += product.count
      }

      for (const i in productList) {
        const product = productList[i]
        if (product.check) {
          result.price += product.count * product.price
        }
      }

      for (const i in productList) {
        const product = productList[i]
        if (product.count > 0 && !product.check) {
          result.allChecked = false
        }
      }
    }
    result.price = result.price.toFixed(2)
    return result
  })

  const useCurrentListEffect = (tab, shopId) => {
    const content = reactive({ list: {} })
    const getContentData = async () => {
      const result = await get(`/shop/${shopId}/products`, { tab: tab.value })
      if (result?.errno === 0 && result?.data) {
        content.list = result.data
      }
    }
    watchEffect(() => { getContentData() })
    return { content }
  }

  const useCartEffect = () => {
    const store = useStore()
    const cartList = store.state.cartList
    return { cartList }
  }

  const changeCartProductItemCheck = (shopId, productId) => {
    store.commit('changeCartProductItemCheck', { shopId, productId })
  }

  const cleanCartProducts = (shopId) => {
    store.commit('cleanCartProducts', { shopId })
    showCart.value = false
  }

  const setCartAllChecked = (shopId) => {
    store.commit('setCartAllChecked', { shopId })
  }

  const handleshowCart = () => {
    if (caculations.value.total > 0) {
      showCart.value = !showCart.value
    } else {
      showCart.value = false
    }
  }

  return {
    showCart,
    shopName,
    productList,
    caculations,
    delCartItem,
    addItemToCart,
    useCartEffect,
    handleshowCart,
    cleanCartProducts,
    setCartAllChecked,
    useCurrentListEffect,
    changeCartProductItemCheck
  }
}

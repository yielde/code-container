import { useStore } from 'vuex'
export const useCommonCartEffect = () => {
  const store = useStore()

  const addItemToCart = (shopId, productId, productInfo, shopName) => {
    store.commit('addItemToCart', { shopId, productId, productInfo })
    addShopName(shopId, shopName)
  }

  const addShopName = (shopId, shopName) => {
    store.commit('addShopName', { shopId, shopName })
  }

  const delCartItem = (shopId, productId) => {
    store.commit('delCartItem', { shopId, productId })
  }

  return { addItemToCart, delCartItem }
}

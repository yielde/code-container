import { useStore } from 'vuex'
export const useCommonCartEffect = () => {
  const store = useStore()

  const addItemToCart = (shopId, productId, productInfo) => {
    store.commit('addItemToCart', { shopId, productId, productInfo })
  }

  const delCartItem = (shopId, productId) => {
    store.commit('delCartItem', { shopId, productId })
  }

  return { addItemToCart, delCartItem }
}

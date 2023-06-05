<template>
  <div class="mask" v-if="showCart" @click="handleshowCart"></div>
  <div class="cart">
    <div class="product" v-show="showCart">
      <div class="product__header">
        <div class="product__header__all">
          <span class="product__header__all__icon iconfont"
            :class="caculations.allChecked ? 'icon-checked' : 'icon-checked1'" @click="setCartAllChecked(shopId)"></span>
          全选
        </div>
        <div class="product__header__clear">
          <span class="product__header__clear__btn" @click="cleanCartProducts(shopId)">清空购物车</span>
        </div>
      </div>
      <!-- template占位判断数量为0时，不展示count内容 -->
      <template v-for=" item  in  productList " :key="item.id">
        <div class="product__item" v-if="item.count > 0">
          <div class="product__item__checked iconfont" :class="item.check ? 'icon-checked' : 'icon-checked1'"
            @click="changeCartProductItemCheck(shopId, item.id)"></div>
          <img class="product__item__img" :src="item.imgUrl" />
          <div class="product__item__detail">
            <h4 class="product__item__detail__title">{{ item.name }}</h4>
            <p class="product__item__detail__price">
              <span class="product__item__detail__yen">&yen;</span>{{ item.price }}
              <span class="product__item__detail__origin">&yen;{{ item.oldPrice }}</span>
            </p>
            <div class="product__item__number">
              <span class="product__item__number__minus" @click="delCartItem(shopId, item.id)">-</span>
              {{ item.count || 0 }}
              <span class="product__item__number__plus" @click="addItemToCart(shopId, item.id, item)">+</span>
            </div>
          </div>
        </div>
      </template>
    </div>
    <div class="bill">
      <div class="bill__icon">
        <img src="../../assets/images/购物车.png" class="bill__icon__img" @click="handleshowCart" />
        <div class="bill__icon__tag">{{ caculations.total }}</div>
      </div>
      <div class="bill__info">总计:<span class="bill__info__price"> &yen;{{ caculations.price }}</span></div>
      <div class="bill__btn">
        <router-link :to="{ name: 'HomePage' }">去结算</router-link>
      </div>
    </div>
  </div>
</template>
<script>
import { computed, ref } from 'vue'
import { useStore } from 'vuex'
import { useRoute } from 'vue-router'

import { useCommonCartEffect } from './commonCartEffect'

const useCartEffect = (shopId) => {
  const store = useStore()
  const cartList = store.state.cartList
  const showCart = ref(false)
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
  const productList = computed(() => {
    const productList = cartList[shopId]?.productList || {}
    return productList
  })

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

  const { addItemToCart, delCartItem } = useCommonCartEffect()
  return {
    caculations,
    showCart,
    handleshowCart,
    productList,
    addItemToCart,
    delCartItem,
    changeCartProductItemCheck,
    cleanCartProducts,
    setCartAllChecked
  }
}

export default {
  name: 'CartPage',
  props: ['shopName'],
  setup () {
    const route = useRoute()
    const shopId = route.params.id
    const {
      caculations,
      showCart,
      handleshowCart,
      productList,
      addItemToCart,
      delCartItem,
      changeCartProductItemCheck,
      cleanCartProducts,
      setCartAllChecked
    } = useCartEffect(shopId)

    return {
      caculations,
      showCart,
      handleshowCart,
      productList,
      addItemToCart,
      delCartItem,
      shopId,
      changeCartProductItemCheck,
      cleanCartProducts,
      setCartAllChecked
    }
  }
}
</script>
<style lang="scss" scoped>
@import '../../style/variables.scss';
@import '../../style/mixins.scss';

.mask {
  position: fixed;
  left: 0;
  right: 0;
  bottom: 0;
  top: 0;
  background-color: rgba(0, 0, 0, 0.50);
}

.cart {
  position: absolute;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: $content-background-color;
}

.bill {
  display: flex;
  height: .49rem;
  line-height: .49rem;
  border-top: .01rem solid $content-background-color;

  &__icon {
    position: relative;
    width: .84rem;

    &__img {
      display: block;
      margin: .12rem .4rem .12rem auto;
      width: .28rem;
      height: .28rem;
    }

    &__tag {
      position: absolute;
      left: .4rem;
      padding: 0 .04rem;
      top: .04rem;
      min-width: .2rem;
      line-height: .2rem;
      height: .2rem;
      background-color: #e93b3b;
      border-radius: .1rem;
      font-size: .12rem;
      text-align: center;
      color: $bg-color;
      transform: scale(.9);
      transform-origin: left center;

    }
  }

  &__info {
    flex: 1;
    color: $content-fontcolor;
    font-size: .12rem;

    &__price {
      color: #e93b3b;
      font-size: .18rem;
    }
  }

  &__btn {
    width: .98rem;
    background-color: #4fb0f9;
    color: $bg-color;
    font-size: .16rem;
    text-align: center;

    a {
      color: $bg-color;
      text-decoration: none;
    }
  }
}

.product {
  flex: 1;
  overflow-y: scroll;
  background-color: $bg-color;
  // 商品过多展示屏幕80%
  max-height: 80vh;

  &__header {
    display: flex;
    line-height: .52rem;
    border-bottom: 1px solid $content-background-color;
    padding: 0 .16rem;
    font-size: .16rem;

    &__all {
      color: $content-fontcolor;

      &__icon {
        color: #0091ff;
        font-size: .2rem;
      }
    }

    &__clear {
      flex: 1;
      text-align: right;
      font-size: .14rem;
      margin-right: .16rem;
      color: $content-fontcolor;

      &__btn {
        display: inline-block;
      }
    }
  }

  &__item {
    display: flex;
    padding: .12rem .16rem;
    position: relative;
    color: $content-fontcolor;

    &__checked {
      color: #0091FF;
      font-size: .2rem;
      line-height: .5rem;
      margin-right: .2rem;
    }

    &__img {
      width: .46rem;
      height: .46rem;
      margin-right: .16rem;
    }

    &__detail {
      overflow: hidden;

      &__title {
        margin: 0;
        line-height: .2rem;
        font-size: .14rem;
        color: $content-fontcolor;
        @include ellipsis;
      }

      &__price {
        margin: .06rem 0 0 0;
        line-height: .2rem;
        font-size: .14rem;
        color: #e93b3b;
      }

      &__yen {
        font-size: .12rem;
      }

      &__origin {
        font-size: .12rem;
        color: #999;
        text-decoration: line-through;
        margin-left: .06rem;
      }
    }

    &__number {
      position: absolute;
      right: .16rem;
      bottom: .12rem;
      line-height: .18rem;

      &__minus {
        display: inline-block;
        width: .2rem;
        height: .2rem;
        border-radius: 50%;
        text-align: center;
        border: .01rem solid #666;
        color: #666;
        font-size: .2rem;
        margin-right: .1rem;
      }

      &__plus {
        display: inline-block;
        width: .22rem;
        height: .22rem;
        border-radius: 50%;
        text-align: center;
        background: #0091FF;
        color: $bg-color;
        font-size: .2rem;
        margin-left: .1rem;
      }
    }
  }
}
</style>

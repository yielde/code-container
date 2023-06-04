<template>
  <div class="cart">
    <div class="bill">
      <div class="bill__icon">
        <img src="../../assets/images/购物车.png" class="bill__icon__img" />
        <div class="bill__icon__tag">{{ total }}</div>
      </div>
      <div class="bill__info">总计:<span class="bill__info__price"> &yen;{{ price }}</span></div>
      <div class="bill__btn">去结算</div>
    </div>
  </div>
</template>
<script>
import { computed } from 'vue'
import { useStore } from 'vuex'
import { useRoute } from 'vue-router'

const useCartEffect = () => {
  const store = useStore()
  const route = useRoute()
  const cartList = store.state.cartList
  const shopId = route.params.id
  const total = computed(() => {
    const productList = cartList[shopId]
    let count = 0
    if (productList) {
      for (const i in productList) {
        const product = productList[i]
        count += product.count
      }
    }
    return count
  })

  const price = computed(() => {
    const productList = cartList[shopId]
    let price = 0
    if (productList) {
      for (const i in productList) {
        const product = productList[i]
        price += product.count * product.price
      }
    }
    return price.toFixed(2)
  })

  return { total, price }
}
export default {
  name: 'CartPage',
  setup () {
    const { total, price } = useCartEffect()
    return { total, price }
  }
}
</script>
<style lang="scss" scoped>
@import '../../style/variables.scss';

.cart {
  position: absolute;
  left: 0;
  right: 0;
  bottom: 0;
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
    font-size: .14rem;
    text-align: center;
  }
}
</style>

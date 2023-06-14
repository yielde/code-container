<template>
  <div class="order">
    <div class="order__price">实付金额 <b>&yen;{{ caculations.price }}</b></div>
    <div class="order__btn">提交订单</div>
  </div>
  <div class="confirm">
    <div class="confirm__content">
      <h3 class="confirm__content__title">收银台</h3>
      <p class="confirm__content__desc">请尽快完成支付</p>
      <div class="confirm__content__btns">
        <div class="confirm__content__cancel" @click="handleCancelOrder">取消订单</div>
        <div class="confirm__content__ensure" @click="handleEnsureOrder">确认支付</div>
      </div>
    </div>
  </div>
</template>
<script>
import { useRoute } from 'vue-router'
import { post } from '../../utils/request'
import { useCommonCartEffect } from './../../effects/cartEffects'

const useOrderEffect = () => {
  const route = useRoute()
  const shopId = route.params.id
  const { caculations, shopName, productList } = useCommonCartEffect(shopId)
  const products = () => {
    const list = []
    for (const i in productList.value) {
      const product = productList.value[i]
      list.push({ id: product.id, num: product.count })
    }
    return list
  }

  const handleCancelOrder = () => {

  }
  const handleEnsureOrder = async () => {
    const result = await post('/order', {
      addressId: 1,
      shopId,
      shopName: shopName.value,
      isCanceld: false,
      products: products()
    })
    if (result.errno === 0) {
      return result
    }
  }

  return { handleCancelOrder, handleEnsureOrder, caculations }
}

export default {
  name: 'OrderPart',
  setup () {
    const { handleCancelOrder, handleEnsureOrder, caculations } = useOrderEffect()
    return { handleCancelOrder, handleEnsureOrder, caculations }
  }
}
</script>
<style lang="scss" scoped>
@import '../../style/variables.scss';

.order {
  position: absolute;
  display: flex;
  height: .49rem;
  line-height: .49rem;
  bottom: 0;
  left: 0;
  right: 0;
  background-color: $bg-color;

  &__price {
    flex: 1;
    text-indent: .24rem;
    font-size: .14rem;
    color: $content-fontcolor;
  }

  &__btn {
    width: .98rem;
    background-color: #4fb0f9;
    color: #fff;
    text-align: center;
    font-size: .14rem;
  }
}

.confirm {
  position: absolute;
  z-index: 1;
  left: 0;
  right: 0;
  top: 0;
  bottom: 0;
  background-color: rgba(0, 0, 0, 0.50);

  &__content {
    position: absolute;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
    width: 3rem;
    height: 1.56rem;
    border-radius: .04rem;
    background: $bg-color;
    text-align: center;

    &__title {
      margin-top: .24rem;
      line-height: .26rem;
      font-size: .18rem;
      color: $content-fontcolor;
    }

    &__desc {
      font-size: .14rem;
      color: #666;
    }

    &__btns {
      margin: .24rem .58rem 0 .58rem;
      display: flex;
      font-size: .14rem;
    }

    &__cancel {
      flex: 1;
      width: .8rem;
      line-height: .32rem;
      border: .01rem solid #4fb0f9;
      border-radius: .16rem;
      margin-right: .12rem;
      color: #4fb0f9;
    }

    &__ensure {
      flex: 1;
      width: .8rem;
      line-height: .32rem;
      border: .01rem solid #4fb0f9;
      border-radius: .16rem;
      margin-left: .12rem;
      background: #4fb0f9;
      color: #fff;
    }
  }
}
</style>

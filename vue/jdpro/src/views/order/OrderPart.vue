<template>
  <div class="order">
    <div class="order__price">实付金额 <b>&yen;{{ caculations.price }}</b></div>
    <div class="order__btn">提交订单</div>
  </div>
</template>
<script>
import { useRoute } from 'vue-router'
import { useCommonCartEffect } from './../../effects/cartEffects'

export default {
  name: 'OrderPart',
  setup () {
    const route = useRoute()
    const shopId = route.params.id
    const { caculations } = useCommonCartEffect(shopId)

    return { caculations }
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
</style>

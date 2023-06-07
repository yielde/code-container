<template>
  <div class="product">
    <h4 class="product__title">{{ shopName }}</h4>
    <div class="product__list">
      <div class="product__item" v-for="item in productList" :key="item.id">
        <img class="product__item__img" :src="item.imgUrl" />
        <div class="product__item__detail">
          <h4 class="product__item__detail__title">{{ item.name }}</h4>
          <p class="product__item__detail__price">
            <span class="product__item__detail__yen">&yen; {{ item.price }} x {{ item.count }}</span>
            <span class="product__item__detail__total">&yen; {{ item.price * item.count }}</span>
          </p>
        </div>
      </div>
    </div>
  </div>
</template>
<script>
import { useRoute } from 'vue-router'
import { useCommonCartEffect } from './../../effects/cartEffects'

export default {
  name: 'ProductPart',
  setup () {
    const route = useRoute()
    const shopId = route.params.id
    const { productList, shopName } = useCommonCartEffect(shopId)
    return { productList, shopName }
  }
}
</script>
<style lang="scss" scoped>
@import '../../style/variables.scss';
@import '../../style/mixins.scss';

.product {
  margin: .16rem .18rem .1rem .18rem;
  background-color: $bg-color;

  &__title {
    margin: 0;
    padding: .16rem;
    font-size: .16rem;
    line-height: .16rem;
    @include ellipsis;
  }

  &__list {
    position: absolute;
    overflow-y: scroll;
    margin: 0 .18rem;
    left: 0;
    right: 0;
    bottom: .6rem;
    top: 2.6rem;
    background-color: $bg-color;
  }

  &__item {
    display: flex;
    padding: .16rem;
    border-bottom: .01rem solid #f1f1f1;
    position: relative;
    color: $content-fontcolor;

    &__img {
      width: .46rem;
      height: .46rem;
      margin-right: .16rem;
    }

    &__detail {
      flex: 1;

      &__title {
        margin: 0;
        line-height: .2rem;
        font-size: .14rem;
        color: $content-fontcolor;
        @include ellipsis;
      }

      &__price {
        line-height: .2rem;
        margin: .06rem 0 0 0;
        font-size: .14rem;
        color: #e93b3b;
        display: flex;
      }

      &__yen {
        flex: 1;
        font-size: .12rem;
      }

      &__total {
        color: #000;
        text-align: right;
      }
    }
  }
}
</style>

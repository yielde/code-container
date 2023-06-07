<template>
  <div class="content">
    <div class="category">
      <div :class="{ 'category__item': true, 'category__item--active': currentTab === item.tab }" v-for="item in category"
        :key="item.tab" @click="handleTabClick(item.tab)">
        {{ item.name }}</div>
    </div>
    <div class="product">
      <div class="product__item" v-for="item in content.list" :key="item.id">
        <img class="product__item__img" :src="item.imgUrl" />
        <div class="product__item__detail">
          <h4 class="product__item__detail__title">{{ item.name }}</h4>
          <p class="product__item__detail__sales">月售 {{ item.sales }} 件</p>
          <p class="product__item__detail__price">
            <span class="product__item__detail__yen">&yen;</span>{{ item.price }}
            <span class="product__item__detail__origin">&yen;{{ item.oldPrice }}</span>
          </p>
          <div class="product__item__number">
            <span class="product__item__number__minus" @click="delCartItem(shopId, item.id)">-</span>
            {{ cartList?.[shopId]?.productList?.[item.id]?.count || 0 }}
            <span class="product__item__number__plus" @click="addItemToCart(shopId, item.id, item, shopName)">+</span>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>
<script>
import { ref } from 'vue'
import { useRoute } from 'vue-router'

import { useCommonCartEffect } from './../../effects/cartEffects'

const category = [{ name: '全部商品', tab: 'all' }, { name: '秒杀', tab: 'seckill' }, { name: '新鲜水果', tab: 'fruit' }]

const useTabEffect = () => {
  const currentTab = ref(category[0].tab)
  const handleTabClick = (tab) => {
    currentTab.value = tab
  }
  return { currentTab, handleTabClick }
}

export default {
  name: 'ContentPart',
  props: ['shopName'],
  setup () {
    const route = useRoute()
    const shopId = route.params.id
    const { addItemToCart, delCartItem, useCurrentListEffect, useCartEffect } = useCommonCartEffect()
    const { currentTab, handleTabClick } = useTabEffect()
    const { content } = useCurrentListEffect(currentTab, shopId)
    const { cartList } = useCartEffect()
    return { category, currentTab, handleTabClick, content, cartList, shopId, addItemToCart, delCartItem }
  }
}
</script>
<style lang="scss" scoped>
@import "../../style/variables.scss";
@import "../../style/mixins.scss";

.content {
  position: absolute;
  display: flex;
  left: 0;
  right: 0;
  top: 1.5rem;
  bottom: .5rem;
}

.category {
  overflow-y: scroll;
  width: .76rem;
  background-color: #f5f5f5;
  height: 100%;

  &__item {
    line-height: .4rem;
    text-align: center;
    font-size: .14rem;
    color: $content-fontcolor;

    &--active {
      background-color: $bg-color;
    }
  }
}

.product {
  flex: 1;
  overflow-y: scroll;

  &__item {
    display: flex;
    padding: .12rem .16rem;
    border-bottom: .01rem solid #f1f1f1;
    position: relative;
    color: $content-fontcolor;

    &__img {
      width: .68rem;
      height: .68rem;
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

      &__sales {
        margin: .06rem 0;
        line-height: .16rem;
        font-size: .12rem;
        color: $content-fontcolor;
      }

      &__price {
        margin: 0;
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

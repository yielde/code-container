<template>
  <div class="wrapper">
    <div class="search">
      <div class="search__back iconfont icon-back" @click="handleBackClick">
      </div>
      <div class="search__content">
        <span class="search__content__icon iconfont icon-search"></span>
        <input class="search__content__input" placeholder="请输入商品名称" />
      </div>
    </div>
    <ShopInfoPart :item="item" v-show="item.imgUrl" />
  </div>
  <contentPart :shopName="item.name" />
  <CartPart />
</template>
<script>
import { reactive, toRefs } from 'vue'
import { useRouter, useRoute } from 'vue-router'
import { get } from '../../utils/request'
import ShopInfoPart from '@/components/ShopInfoPart'
import ContentPart from './ContentPart'
import CartPart from './CartPart'

const useShopInfoEffect = () => {
  // 路由路径相关信息
  const route = useRoute()
  const data = reactive({ item: {} })
  const getItemData = async () => {
    const result = await get(`/shop/${route.params.id}`)
    if (result?.errno === 0 && result?.data) {
      data.item = result.data
    }
  }
  getItemData()
  const { item } = toRefs(data)
  return { item }
}

const useBackRouteEffect = () => {
  const router = useRouter()
  const handleBackClick = () => {
    // 返回上一页
    router.back()
  }
  return { handleBackClick }
}

export default {
  name: 'ShopPage',
  components: { ShopInfoPart, ContentPart, CartPart },
  setup () {
    const { handleBackClick } = useBackRouteEffect()
    const { item } = useShopInfoEffect()
    return { item, handleBackClick }
  }
}
</script>
<style lang="scss" scoped>
@import '../../style/variables.scss';

.wrapper {
  padding: 0 .18rem .02rem .18rem;
}

.search {
  display: flex;
  margin: .14rem 0 .04rem 0;
  line-height: .32rem;

  &__back {
    width: .3rem;
    font-size: .24rem;
    color: #b6b6b6;
  }

  &__content {
    display: flex;
    flex: 1;
    background: #f5f5f5;
    border-radius: .16rem;

    &__icon {
      width: .44rem;
      text-align: center;
      color: #b7b7b7;
      font-size: .2rem;
    }

    &__input {
      display: block;
      width: 100%;
      padding-right: .2rem;
      border: none;
      outline: none;
      background: none;
      color: $content-fontcolor;
    }
  }
}
</style>

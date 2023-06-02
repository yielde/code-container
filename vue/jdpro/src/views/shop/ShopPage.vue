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
    <ShopInfoPart :item="item" />
  </div>
</template>
<script>
import { useRouter } from 'vue-router'
import ShopInfoPart from '@/components/ShopInfoPart'
export default {
  name: 'ShopPage',
  components: { ShopInfoPart },
  setup () {
    const item = {
      slogan: 'VIP尊享满89元减4元运费券（每月3张）',
      expressPrice: 18,
      id: 40,
      name: '沃尔玛',
      imgUrl: 'http://www.dell-lee.com/imgs/vue3/near.png',
      sales: 19,
      expressLimit: 73
    }
    const router = useRouter()
    const handleBackClick = () => {
      // 返回上一页
      router.back()
    }

    return { item, handleBackClick }
  }
}
</script>
<style lang="scss" scoped>
.wrapper {
  padding: 0 .18rem;
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
      color: #333333;
    }
  }
}
</style>

<template>
  <div class="nearby">
    <h2 class="nearby__title">附近店铺</h2>
    <div class="nearby__item" v-for="item in nearbyList" :key="item.id">
      <img :src="item.imgUrl" class="nearby__item__img" />
      <div class="nearby__item__content">
        <div class="nearby__item__content__title">{{ item.name }}</div>
        <div class="nearby__item__content__tags">
          <span class="nearby__item__content__tag">月售: {{ item.sales }}</span>
          <span class="nearby__item__content__tag">起送: ￥{{ item.expressLimit }}</span>
          <span class="nearby__item__content__tag">基础运费: ￥{{ item.expressPrice }}</span>
        </div>
        <p class="nearby__item__content__highlight">
          {{ item.slogan }}
        </p>
      </div>
    </div>
  </div>
</template>

<script>
import { get } from '../../utils/request'
import { ref } from 'vue'

const useNearbyListEffect = () => {
  const nearbyList = ref([])
  const getNearbyList = async () => {
    const result = await get('/shop/hot-list')
    if (result?.errno === 0 && result?.data?.length) {
      nearbyList.value = result.data
    }
  }
  getNearbyList()
  return { nearbyList }
}

export default {
  name: 'NearbyPart',
  setup () {
    const { nearbyList } = useNearbyListEffect()

    return { nearbyList }
  }
}
</script>
<style lang="scss" scoped>
@import "../../style/variables.scss";
@import "../../style/mixins.scss";

.nearby {
  &__title {
    margin: 0.16rem 0 0.02rem 0;
    font-size: 0.18rem;
    font-weight: normal;
    color: $content-fontcolor;
  }

  &__item {
    display: flex;
    padding-top: 0.12rem;

    &__img {
      width: 0.56rem;
      height: 0.56rem;
      margin-right: 0.16rem;
    }

    &__content {
      flex: 1;
      border-bottom: 0.01rem solid $content-background-color;
      padding-bottom: 0.12rem;

      &__title {
        line-height: 0.22rem;
        font-size: 0.16rem;
        color: $content-fontcolor;
      }

      &__tags {
        margin-top: 0.08rem;
        line-height: 0.18rem;
        font-size: 0.13rem;
        color: $content-fontcolor;
      }

      &__tag {
        margin-right: 0.16rem;
      }

      &__highlight {
        color: #e93b3b;
        line-height: 0.18rem;
        font-size: 0.13rem;
        margin: 0.08rem 0 0 0;
      }
    }
  }
}
</style>

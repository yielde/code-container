<template>
  <div class="nearby">
    <h2 class="nearby__title">附近店铺</h2>
    <router-link :to="`/shop/${item.id}`" v-for="item in nearbyList" :key="item.id">
      <ShopInfoPart :item="item" :showBorder="true" />
    </router-link>
  </div>
</template>

<script>
import { ref } from 'vue'
import { get } from '../../utils/request'
import ShopInfoPart from '../../components/ShopInfoPart'

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
  components: { ShopInfoPart },
  setup () {
    const { nearbyList } = useNearbyListEffect()

    return { nearbyList }
  }
}
</script>
<style lang="scss" scoped>
@import "../../style/variables.scss";

.nearby {
  &__title {
    margin: 0.16rem 0 0.02rem 0;
    font-size: 0.18rem;
    font-weight: normal;
    color: $content-fontcolor;
  }

  a {
    text-decoration: none;
  }
}
</style>

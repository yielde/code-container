<template>
  <div class="nearby">
    <h2 class="nearby__title">附近店铺</h2>
    <div class="nearby__item" v-for="item in nearbyList" :key="item.id">
      <img :src="item.img" class="nearby__item__img" />
      <div class="nearby__item__content">
        <div class="nearby__item__content__title">{{ item.title }}</div>
        <div class="nearby__item__content__tags">
          <span class="nearby__item__content__tag" v-for="(tag,index) in item.tags" :key="index">{{ tag }}</span>
        </div>
        <p class="nearby__item__content__highlight">
         {{ item.desc }}
        </p>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'NearbyPart',
  setup () {
    const nearbyList = [
      {
        id: 1,
        title: '沃尔玛',
        tags: ['月售1万', '起送¥0', '基础运费¥5'],
        desc: 'VIP尊享满89元减4元运费券（每月3张）',
        img: 'http://www.dell-lee.com/imgs/vue3/near.png'
      },
      {
        id: 2,
        title: '沃尔玛',
        tags: ['月售1万', '起送¥0', '基础运费¥15'],
        desc: 'VIP尊享满89元减4元运费券（每月2张）',
        img: 'http://www.dell-lee.com/imgs/vue3/near.png'
      },
      {
        id: 3,
        title: '沃尔玛',
        tags: ['月售1万', '起送¥0', '基础运费¥25'],
        desc: 'VIP尊享满89元减4元运费券（每月5张）',
        img: 'http://www.dell-lee.com/imgs/vue3/near.png'
      },
      {
        id: 4,
        title: '沃尔玛',
        tags: ['月售1万', '起送¥0', '基础运费¥53'],
        desc: 'VIP尊享满89元减4元运费券（每月12张）',
        img: 'http://www.dell-lee.com/imgs/vue3/near.png'
      }
    ]
    return { nearbyList }
  }
}
</script>
<style lang="scss">
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

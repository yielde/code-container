<template>
  <div class="shop">
    <img :src="item.imgUrl" class="shop__img" />
    <div :class="{ 'shop__content': true, 'shop__content--showBorder': true ? showBorder : false }">
      <div class="shop__content__title">{{ item.name }}</div>
      <div class="shop__content__tags">
        <span class="shop__content__tag">月售: {{ item.sales }}</span>
        <span class="shop__content__tag">起送: &yen;{{ item.expressLimit }}</span>
        <span class="shop__content__tag">基础运费: &yen;{{ item.expressPrice }}</span>
      </div>
      <p class="shop__content__highlight">
        {{ item.slogan }}
      </p>
    </div>
  </div>
</template>
<script>
export default {
  name: 'ShopInfoPart',
  props: ['item', 'showBorder']
}
</script>
<style lang="scss" scoped>
@import "../style/variables.scss";
@import "../style/mixins.scss";

.shop {
  display: flex;
  padding-top: 0.12rem;

  &__img {
    width: 0.56rem;
    height: 0.56rem;
    margin-right: 0.16rem;
  }

  &__content {
    flex: 1;
    padding-bottom: 0.12rem;

    &--showBorder {
      border-bottom: 0.01rem solid $content-background-color;
    }

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
</style>

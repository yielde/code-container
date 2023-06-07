<template>
  <div class="docker">
    <div v-for="(item, index) in dockerList" :key="item.icon"
      :class="{ 'docker__item': true, 'docker__item--active': index === index }">
      <router-link :to="{ name: item.to.name }">
        <div class="iconfont" :class="item.icon"></div>
        <div class="docker__title">{{ item.text }}</div>
      </router-link>
    </div>
  </div>
</template>
<script>
export default {
  name: 'DockerPart',
  setup () {
    const dockerList = [
      { icon: 'icon-home', text: '首页', to: { name: 'HomePage' } },
      { icon: 'icon-cart', text: '购物车', to: { name: 'CartPage' } },
      { icon: 'icon-order', text: '订单', to: { name: 'HomePage' } },
      { icon: 'icon-mine', text: '我的', to: { name: 'HomePage' } }
    ]
    return { dockerList }
  }
}
</script>
<style lang="scss" scoped>
@import "../../style/variables.scss";

.docker {
  display: flex;
  box-sizing: border-box;
  position: absolute;
  padding: 0 0.18rem;
  left: 0;
  bottom: 0;
  right: 0;
  height: 0.49rem;
  border-top: 0.01rem solid $content-background-color;

  &__item {
    flex: 1;
    text-align: center;

    .iconfont {
      font-size: 0.18rem;
      margin: 0.07rem 0 0.02rem 0;
    }

    a {
      color: $content-fontcolor;
      text-decoration: none;
    }

    &--active {
      a {
        color: #1fa4fc;
      }
    }
  }

  &__title {
    font-size: 0.2rem;
    // 浏览器最小像素12px，10px将20px做缩放
    transform: scale(0.5, 0.5);
    transform-origin: center top;
  }
}
</style>

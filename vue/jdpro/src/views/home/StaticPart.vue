<template>
  <div class="position">
    <span class="iconfont icon-position position__icon"></span>
    迪士尼乐园迪士尼乐园迪士尼乐园迪士尼乐园迪士尼乐园
    <span class="iconfont icon-bell position__notice"></span>
  </div>
  <div class="search">
    <i class="iconfont icon-search search__search-icon"></i>
    <input class="search__input" placeholder="miaomiao会员商城"/>
  </div>
  <div class="banner">
    <img class="banner__img" src="http://www.dell-lee.com/imgs/vue3/banner.jpg" />
  </div>
  <div class="icons">
    <div class="icons__item" v-for="icon in iconsList" :key="icon.id">
      <img class="icons__item__img" :src="icon.img" />
      <p class="icons__item__desc">{{ icon.desc }}</p>
    </div>
  </div>
  <div class="gap"></div>
</template>
<script>
export default {
  name: 'StaticPart',
  data () {
    return {
      imagePath: process.env.BASE_URL
    }
  },
  setup () {
    const iconsList = [
      // 使用require添加本地静态资源，否则会被webpack处理为链接
      { id: 1, img: require('../../assets/images/超市.png'), desc: '超市便利' },
      { id: 1, img: require('../../assets/images/菜市场.png'), desc: '菜市场' },
      { id: 1, img: require('../../assets/images/水果店.png'), desc: '水果店' },
      { id: 1, img: require('../../assets/images/鲜花.png'), desc: '鲜花' },
      { id: 1, img: require('../../assets/images/医药健康.png'), desc: '医药健康' },
      { id: 1, img: require('../../assets/images/家居.png'), desc: '家居' },
      { id: 1, img: require('../../assets/images/蛋糕.png'), desc: '蛋糕' },
      { id: 1, img: require('../../assets/images/签到.png'), desc: '签到' },
      { id: 1, img: require('../../assets/images/大牌免运.png'), desc: '大牌免运' },
      { id: 1, img: require('../../assets/images/红包.png'), desc: '红包' }
    ]
    return { iconsList }
  }
}
</script>
<style lang="scss" scoped>
@import "../../style/variables.scss";
@import "../../style/mixins.scss";

.position {
  position: relative;
  padding: 0.16rem 0.24rem 0.16rem 0;
  line-height: 0.22rem;
  font-size: 0.18rem;
  color: $content-fontcolor;
  @include ellipsis;

  .position__icon {
    position: relative;
    top: 0.01rem;
    font-size: 0.18rem;
  }

  .position__notice {
    position: absolute;
    top: 0.17rem;
    right: 0;
    font-size: 0.18rem;
  }
}

.search {
  position: relative;
  box-sizing: border-box;
  padding: 0 0.352rem 0 0;
  font-size: 0.14rem;
  margin-bottom: 0.14rem;

  &__input {
    color: #b7b7b7;
    line-height: 0.32rem;
    background: #f5f5f5;
    border: none;
    outline: none;
    border-radius: 0.16rem;
    padding-left: 0.3rem;
    width: 100%;
  }

  &__search-icon {
    color: #b7b7b7;
    position: absolute;
    line-height: 0.32rem;
    margin: 0.03rem 0 0 0.1rem;
  }
}

.banner {
  height: 0;
  overflow: hidden;
  padding-bottom: 25.4%;

  &__img {
    width: 100%;
  }
}

.icons {
  display: flex;
  flex-wrap: wrap;
  margin-top: 0.16rem;

  &__item {
    width: 20%;
    text-align: center;

    &__img {
      display: block;
      width: 0.4rem;
      height: 0.4rem;
      margin: 0 auto;
    }

    &__desc {
      text-align: center;
      margin: 0.06rem 0 0.16rem 0;
      color: $content-fontcolor;
    }
  }
}

.gap {
  height: 0.1rem;
  background: $content-background-color;
  margin: 0 -0.18rem;
}
</style>

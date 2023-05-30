<template>
  <div class="wrapper">
    <div class="position">
      <span class="iconfont icon-position position__icon"></span>
      迪士尼乐园迪士尼乐园迪士尼乐园迪士尼乐园迪士尼乐园
      <span class="iconfont icon-bell position__notice"></span>
    </div>
    <div class="search">
      <i class="iconfont icon-search search__search-icon"></i>
      <input class="search__input" value="miaomiao会员商城" />
    </div>
    <div class="banner">
      <img
        class="banner__img"
        src="http://www.dell-lee.com/imgs/vue3/banner.jpg"
      />
    </div>
    <div class="icons">
      <div class="icons__item">
        <img class="icons__item__img" src="./assets/images/超市.png" />
        <p class="icons__item__desc">超市便利</p>
      </div>
      <div class="icons__item">
        <img class="icons__item__img" src="./assets/images/菜市场.png" />
        <p class="icons__item__desc">菜市场</p>
      </div>
      <div class="icons__item">
        <img class="icons__item__img" src="./assets/images/水果店.png" />
        <p class="icons__item__desc">水果店</p>
      </div>
      <div class="icons__item">
        <img class="icons__item__img" src="./assets/images/鲜花.png" />
        <p class="icons__item__desc">鲜花</p>
      </div>
      <div class="icons__item">
        <img class="icons__item__img" src="./assets/images/医药健康.png" />
        <p class="icons__item__desc">医药</p>
      </div>
      <div class="icons__item">
        <img class="icons__item__img" src="./assets/images/家居.png" />
        <p class="icons__item__desc">家居</p>
      </div>
      <div class="icons__item">
        <img class="icons__item__img" src="./assets/images/蛋糕.png" />
        <p class="icons__item__desc">蛋糕</p>
      </div>
      <div class="icons__item">
        <img class="icons__item__img" src="./assets/images/签到.png" />
        <p class="icons__item__desc">签到</p>
      </div>
      <div class="icons__item">
        <img class="icons__item__img" src="./assets/images/大牌免运.png" />
        <p class="icons__item__desc">大牌免运</p>
      </div>
      <div class="icons__item">
        <img class="icons__item__img" src="./assets/images/红包.png" />
        <p class="icons__item__desc">红包</p>
      </div>
    </div>
    <div class="gap"></div>
    <div class="nearby">
      <h2 class="nearby__title">附近店铺</h2>
      <div class="nearby__item">
        <img
          src="http://www.dell-lee.com/imgs/vue3/near.png"
          class="nearby__item__img"
        />
        <div class="nearby__item__content">
          <div class="nearby__item__content__title">沃尔玛</div>
          <div class="nearby__item__content__tags">
            <span class="nearby__item__content__tag">月售1万+</span>
            <span class="nearby__item__content__tag">起送¥0</span>
            <span class="nearby__item__content__tag">基础运费¥5</span>
          </div>
          <p class="nearby__item__content__highlight">
            VIP尊享满89元减4元运费券（每月3张）
          </p>
        </div>
      </div>
      <div class="nearby__item">
        <img
          src="http://www.dell-lee.com/imgs/vue3/near.png"
          class="nearby__item__img"
        />
        <div class="nearby__item__content">
          <div class="nearby__item__content__title">沃尔玛</div>
          <div class="nearby__item__content__tags">
            <span class="nearby__item__content__tag">月售1万+</span>
            <span class="nearby__item__content__tag">起送¥0</span>
            <span class="nearby__item__content__tag">基础运费¥5</span>
          </div>
          <p class="nearby__item__content__highlight">
            VIP尊享满89元减4元运费券（每月3张）
          </p>
        </div>
      </div>
      <div class="nearby__item">
        <img
          src="http://www.dell-lee.com/imgs/vue3/near.png"
          class="nearby__item__img"
        />
        <div class="nearby__item__content">
          <div class="nearby__item__content__title">沃尔玛</div>
          <div class="nearby__item__content__tags">
            <span class="nearby__item__content__tag">月售1万+</span>
            <span class="nearby__item__content__tag">起送¥0</span>
            <span class="nearby__item__content__tag">基础运费¥5</span>
          </div>
          <p class="nearby__item__content__highlight">
            VIP尊享满89元减4元运费券（每月3张）
          </p>
        </div>
      </div>
      <div class="nearby__item">
        <img
          src="http://www.dell-lee.com/imgs/vue3/near.png"
          class="nearby__item__img"
        />
        <div class="nearby__item__content">
          <div class="nearby__item__content__title">沃尔玛</div>
          <div class="nearby__item__content__tags">
            <span class="nearby__item__content__tag">月售1万+</span>
            <span class="nearby__item__content__tag">起送¥0</span>
            <span class="nearby__item__content__tag">基础运费¥5</span>
          </div>
          <p class="nearby__item__content__highlight">
            VIP尊享满89元减4元运费券（每月3张）
          </p>
        </div>
      </div>
    </div>
  </div>
  <div class="docker">
    <div class="docker__item docker__item--active">
      <div class="iconfont icon-home"></div>
      <div class="docker__title">首页</div>
    </div>
    <div class="docker__item">
      <div class="iconfont icon-cart"></div>
      <div class="docker__title">购物车</div>
    </div>
    <div class="docker__item">
      <div class="iconfont icon-order"></div>
      <div class="docker__title">订单</div>
    </div>
    <div class="docker__item">
      <div class="iconfont icon-mine"></div>
      <div class="docker__title">我的</div>
    </div>
  </div>
</template>

<style lang="scss">
@import "./style/variables.scss";
@import "./style/mixins.scss";
.wrapper {
  overflow-y: auto;
  position: absolute;
  top: 0;
  right: 0;
  bottom: 0.5rem;
  left: 0;
  padding: 0 0.18rem .1rem .18rem;
}

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
  color: $content-fontcolor;
  &__item {
    flex: 1;
    text-align: center;
    .iconfont {
      font-size: 0.18rem;
      margin: 0.07rem 0 0.02rem 0;
    }
    &--active {
      color: #1fa4fc;
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

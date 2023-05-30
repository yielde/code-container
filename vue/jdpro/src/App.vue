<template>
  <div class="docker">
    <span class="docker__item">首页</span>
    <span class="docker__item">购物车</span>
    <span class="docker__item">订单</span>
    <span class="docker__item">我的</span>
  </div>
</template>

<style lang="scss">
.docker {
  display: flex;
  box-sizing: border-box;
  position: absolute;
  padding: 0 .18rem;
  left: 0;
  bottom: 0;
  width: 100%;
  height: .49rem;
  border-top: 1px solid #ffffff;
  background: green;
}
.docker__item {
  flex: 1;
  text-align: center;
}
</style>

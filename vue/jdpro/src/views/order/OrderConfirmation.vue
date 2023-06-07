<template>
  <div class="wrapper">
    <TopPart />
    <ProductPart />
    <OrderPart />
  </div>
</template>
<script>
import ProductPart from './ProductPart'
import TopPart from './TopPart'
import OrderPart from './OrderPart'

export default {
  name: 'OrderConfirmation',
  components: { TopPart, OrderPart, ProductPart }
}
</script>
<style lang="scss" scoped>
@import '../../style/variables.scss';
@import '../../style/mixins.scss';

.wrapper {
  position: absolute;
  background-color: #eee;
  left: 0;
  right: 0;
  top: 0;
  bottom: 0;
}
</style>

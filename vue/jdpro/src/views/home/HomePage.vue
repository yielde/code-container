<template>
  <div class="wrapper">
    <StaticPart />
    <NearbyPart />
  </div>
  <DockerPart />
</template>
<script>
import StaticPart from './StaticPart'
import NearbyPart from './NearbyPart.vue'
import DockerPart from './DockerPart.vue'
export default {
  name: 'HomePage',
  components: {
    StaticPart,
    NearbyPart,
    DockerPart
  }
}
</script>
<style lang="scss" scoped>
@import "../../style/variables.scss";
@import "../../style/mixins.scss";

.wrapper {
  overflow-y: auto;
  position: absolute;
  top: 0;
  right: 0;
  bottom: 0.5rem;
  left: 0;
  padding: 0 0.18rem 0.1rem 0.18rem;
}
</style>

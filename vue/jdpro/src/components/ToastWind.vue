<template>
  <div class="toast">{{ message }}</div>
</template>
<script>
import { reactive } from 'vue'
export default {
  name: 'ToastWind',
  props: ['message']
}

export const useToastEffect = () => {
  const toastData = reactive({ showToast: false, toastMessage: '' })
  const showToast = (message) => {
    toastData.showToast = true
    toastData.toastMessage = message
    setTimeout(() => {
      toastData.showToast = false
      toastData.toastMessage = ''
    }, 2000)
  }
  return { toastData, showToast }
}
</script>
<style lang="scss" scoped>
@import '../style/variables.scss';

.toast {
  position: fixed;
  left: 50%;
  top: 50%;
  transform: translate(-50%, -50%);
  background: rgba(0, 0, 0, .35);
  border-radius: .05rem;
  color: $bg-color;
  padding: .1rem;
}
</style>

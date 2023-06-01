<template>
  <div class="wrapper">
    <img class="wrapper__img" src="http://www.dell-lee.com/imgs/vue3/user.png">
    <div class="wrapper__input">
      <input class="wrapper__input__content" placeholder="请输入手机号" v-model="data.username" />
    </div>
    <div class="wrapper__input">
      <input class="wrapper__input__content" placeholder="请输入密码" v-model="data.password" />
    </div>
    <div class="wrapper__login-button" @click="handleLogin">登录</div>
    <div class="wrapper__login-link" @click="handleToRegister">没有帐号？去注册</div>
  </div>
  <ToastWind v-if="toastData.showToast" :message="toastData.toastMessage" />
</template>
<script>
import { useRouter } from 'vue-router'
import { reactive } from 'vue'

import { post } from '../../utils/request'
import ToastWind, { useToastEffect } from '../../components/ToastWind.vue'

const useLoginEffect = (showToast) => {
  const router = useRouter()

  const data = reactive({ username: '', password: '' })
  const handleLogin = async () => {
    try {
      const result = await post('/user/login', { username: data.username, password: data.password })
      if (result?.errno === 0) {
        localStorage.isLogin = true
        router.push({ name: 'HomePage' })
      } else {
        showToast('登录失败')
      }
    } catch (e) {
      showToast('请求失败')
    }
  }
  return { handleLogin, data }
}

const useToRegisterEffect = () => {
  const router = useRouter()
  const handleToRegister = () => {
    router.push({ name: 'RegisterPage' })
  }
  return { handleToRegister }
}

export default {
  name: 'LoginPage',
  components: { ToastWind },
  setup () {
    const { toastData, showToast } = useToastEffect()
    const { handleLogin, data } = useLoginEffect(showToast)
    const { handleToRegister } = useToRegisterEffect()
    return { handleLogin, handleToRegister, data, toastData }
  }
}
</script>
<style lang="scss" scoped>
.wrapper {
  position: absolute;
  top: 50%;
  left: 0;
  right: 0;
  transform: translateY(-50%);

  &__img {
    width: .66rem;
    height: .66rem;
    display: block;
    margin: 0 auto .4rem auto;
  }

  &__input {
    background-color: #F9F9F9;
    border: .01rem solid rgba(0, 0, 0, 0.10);
    border-radius: .06rem;
    border-radius: .06rem;
    height: .48rem;
    margin: 0 .4rem .16rem .4rem;
    padding: 0 .16rem;

    &__content {
      background: none;
      border: none;
      line-height: .48rem;
      width: 100%;
      font-size: .16rem;

      &::placeholder {
        color: #777777;
      }
    }
  }

  &__login-button {
    margin: .32rem .4rem .16rem .4rem;
    line-height: .48rem;
    font-size: .16rem;
    text-align: center;
    background: #0091FF;
    box-shadow: 0 .04rem .08rem 0 rgba(0, 145, 255, 0.32);
    border-radius: .04rem;
  }

  &__login-link {
    font-size: .14rem;
    color: #777777;
    text-align: center;
  }
}
</style>

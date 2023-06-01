<template>
  <div class="wrapper">
    <img class="wrapper__img" src="http://www.dell-lee.com/imgs/vue3/user.png" />
    <div class="wrapper__input">
      <input class="wrapper__input__content" placeholder="用户名" />
    </div>
    <div class="wrapper__input">
      <input class="wrapper__input__content" placeholder="密码" />
    </div>
    <div class="wrapper__input">
      <input class="wrapper__input__content" placeholder="确认密码" />
    </div>
    <div class="wrapper__button">注册</div>
    <div class="wrapper__register-link" @click="handleToLogin">已有用户？去登录</div>
  </div>
</template>
<script>
import { useRouter } from 'vue-router'
export default {
  name: 'RegisterPage',
  setup () {
    const router = useRouter()
    const handleToLogin = () => {
      router.push('/login')
    }
    return { handleToLogin }
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
    display: block;
    width: .66rem;
    height: .66rem;
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

      &::placeholder {
        color: #777777;
      }
    }
  }

  &__button {
    margin: .32rem .4rem .16rem .4rem;
    line-height: .48rem;
    font-size: .16rem;
    text-align: center;
    background: #0091FF;
    box-shadow: 0 .04rem .08rem 0 rgba(0, 145, 255, 0.32);
    border-radius: .04rem;
  }

  &__register-link {
    font-size: .14rem;
    color: #777777;
    text-align: center;
  }
}
</style>

<template>
    <NavBarMenu/>
    <SiderBarMenu/>
    >
</template>

<script>
import NavBarMenu from './components/NavBarMenu.vue'
import SiderBarMenu from './components/SiderBarMenu.vue';

export default {
  name: 'App',
  components: {
    NavBarMenu,
    SiderBarMenu
}
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>

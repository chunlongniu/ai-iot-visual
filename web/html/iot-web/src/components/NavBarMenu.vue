<template>
    <nav class="navbar bg-dark navbar-expand-lg bg-body-tertiary" data-bs-theme="dark">
      <div class="container-fluid"> 
          <a class="navbar-brand" href="#">{{ logo  }}</a>
          <div class="collapse navbar-collapse" id="navbarScroll">
              <ul class="navbar-nav me-auto my-2 my-lg-0 navbar-nav-scroll" style="--bs-scroll-height: 100px;">
                  <li class="nav-item">
                      <a class="nav-link active" aria-current="page" href="#">{{ home }}</a>
                  </li>
                  <li class="nav-item">
                      <a class="nav-link" href="#">{{ link }}</a>
                  </li>
              </ul>
            </div>
      </div>
    </nav>
</template>

<script>

export default {
    name: 'MavBarMenu', 
    props: {
        msg: String
    },
    data() {
        return {
            logo: "IOT WebUI",
            home: "Home",
            link: "Link",
        }
    }
}

</script>

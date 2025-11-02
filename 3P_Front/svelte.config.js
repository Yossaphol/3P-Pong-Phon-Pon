import adapter from '@sveltejs/adapter-static';

const config = {
  kit: {
    adapter: adapter({
      fallback: null
    }),
    paths: {
      base: '/3P-Pong-Phon-Pon',
    },
  }
};

export default config;
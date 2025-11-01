import { sveltekit } from '@sveltejs/kit/vite';

/** @type {import('vite').UserConfig} */
const config = {
  plugins: [sveltekit()],
  // 👇 จุดสำคัญมาก
  base: './'
};

export default config;
<script>
    import Pcard from "./pcard.svelte";
    import Mcard from "./mcard.svelte";
</script>

<div class="mt-20 ">
    <h1 class="antialiased text-3xl font-bold text-center mt-10 text-[#2550FF]">
        Promotion manager
    </h1>
    <h4 class="text-center antialiased text-sm text-[#868686] mt-2">
        ใช้สิ่งนี้เพื่อทดสอบระบบ
    </h4>
    <div class="mt-6 z-10 relative flex justify-center flex-wrap gap-4">
        <Pcard></Pcard>
        <Mcard></Mcard>
        <div 
        class="absolute bottom-0 left-0 w-full h-2/3
        bg-linear-to-t from-sky-500 to-indigo-500
        rounded-tl-[800px]
        rounded-br-[800px]
        opacity-40
        z-0
        blur-3xl">
        </div>
    </div>
</div>
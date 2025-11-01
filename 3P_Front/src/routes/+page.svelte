<script lang="ts">
    import Mynav from '../components/nav.svelte';
    import Test from '../components/test.svelte';
    import favicon from '$lib/assets/favicon.svg';
    import tail from '../tailwind-css.svg';
    import p1 from '../67070100.jpeg';
    import p2 from '../67070118.jpg';
    import p3 from '../67070145.jpg';
    import p4 from '../67070166.jpg';
    import img2 from '../IMG_6180.jpg';
    import img3 from '../IMG_6181.jpg';

    let mainImg1:HTMLImageElement;
    let mainImg2:HTMLImageElement;
    let isFading = false; // state ควบคุม fade
    async function changeImg(index:number) {
      isFading = true;
      if (index == 2) {
        await new Promise((r) => setTimeout(r, 300));
        mainImg1.style = '';
        mainImg2.style = 'display: none;';
      }
      else if (index == 3) {
        await new Promise((r) => setTimeout(r, 300));
        mainImg1.style = 'display: none;';
        mainImg2.style = '';
      }
      isFading = false;
    }
    // const mainImage = document.getElementById("mainImage");
</script>

<Mynav></Mynav>
<Test></Test>

<div class="flex flex-col items-center py-8 mt-10">
    <div class="w-0 h-0 border-l-8 border-l-transparent border-r-8 border-r-transparent border-t-8 border-t-blue-400 mb-2"></div>
    <div class="w-0 h-0 border-l-8 border-l-transparent border-r-8 border-r-transparent border-t-8 border-t-blue-400 mb-2"></div>
    <div class="w-0 h-0 border-l-8 border-l-transparent border-r-8 border-r-transparent border-t-8 border-t-blue-600"></div>
</div>

<!-- ส่วนล่าง: Project Details -->
  <main class="bg-white py-16">
    <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
      <h2 class="text-4xl font-extrabold text-blue-700 text-center mb-16">Project details</h2>
      
      <!-- Project Details Section 1 -->
      <section class="bg-white rounded-lg shadow-xl p-8 mb-12 border border-gray-200">
        <iframe class="mx-auto w-9/10 h-48 md:h-100 lg:h-150 mb-4 rounded-lg" src="https://www.youtube.com/embed/FyEzlFBqOGs?si=VTTOl8uyEW5gmaRC" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>
        <h3 class="text-2xl font-semibold text-gray-800 mb-6">รายละเอียด</h3>
        <p class="text-gray-700 leading-relaxed mb-6">&nbsp;&nbsp;&nbsp;โครงงานนี้มีวัตถุประสงค์เพื่อพัฒนา ระบบ Captive Portal สำหรับการยืนยันตัวตนผ่าน LINE บนบอร์ด ESP32 โดยให้ ESP32 ทำหน้าที่เป็น เราเตอร์ขนาดเล็ก (NAT Router) ที่สามารถปล่อยสัญญาณ Wi-Fi ให้ผู้ใช้งานเชื่อมต่อ และบังคับให้เข้าสู่หน้าเข้าสู่ระบบ (Login Page) ก่อนใช้งานอินเทอร์เน็ตได้จริง ระบบถูกออกแบบให้มีการ เชื่อมต่อกับแพลตฟอร์ม LINE เพื่อใช้เป็นช่องทางในการยืนยันตัวตนของผู้ใช้ โดยเมื่อผู้ใช้ทำการ Login ผ่าน LINE สำเร็จ ข้อมูลผู้ใช้ เช่น userId และ displayName จะถูกส่งกลับมายัง ESP32 ผ่าน MQTT Broker เพื่อให้ ESP32 เปิดการใช้งานอินเทอร์เน็ตแบบถาวร พร้อมทั้งแสดงชื่อผู้ใช้บนจอ LCD และส่งเสียงแจ้งเตือนผ่าน Buzzer</p>
        <p class="text-gray-700 leading-relaxed mb-6">&nbsp;&nbsp;&nbsp;ประโยชน์ของโครงงานนี้คือสามารถนำไปใช้ในร้านค้า ร้านกาแฟ หรือพื้นที่บริการสาธารณะ เพื่อควบคุมการเข้าใช้งาน Wi-Fi พร้อมเก็บข้อมูลลูกค้าและส่งโปรโมชั่นผ่าน LINE OA ได้โดยอัตโนมัติ อีกทั้งยังเป็นการต่อยอดขีดความสามารถของ ESP32 ให้เป็น Gateway อัจฉริยะ (Smart IoT Gateway) ที่รวม NAT, DNS, HTTP, MQTT, LCD และระบบแจ้งเตือนไว้ในอุปกรณ์เดียว ช่วยลดต้นทุนและเพิ่มความสะดวกในการติดตั้งใช้งานจริง </p>

        <!-- Image Carousel/Gallery Placeholder -->
        <div class="bg-gray-300 rounded-s-md mb-6 flex">
          <img class:opacity-0={isFading} bind:this={mainImg1} class="mx-auto justify-center transition-opacity duration-500 ease-in-out relative w-120 rounded-lg items-center bg-center bg-contain bg-no-repeat" src={img2} alt="img">
          <img style="display: none;" class:opacity-0={isFading} bind:this={mainImg2} class="mx-auto justify-center transition-opacity duration-500 ease-in-out relative w-120 rounded-lg items-center bg-center bg-contain bg-no-repeat" src={img3} alt="img">
        </div>

        <div class="flex justify-center space-x-4">
          <button onclick={() => changeImg(2)} class="thumbnail w-20 h-12 rounded-md cursor-pointer transition-colors duration-200">
            <img class="rounded-md " src={img2} alt="">
          </button>
          <button onclick={() => changeImg(3)} class="thumbnail w-20 h-12 rounded-md cursor-pointer transition-colors duration-200">
            <img class="rounded-md" src={img3} alt="">
          </button>
        </div>

      </section>

      <!-- Project Details Section 2 (เพิ่มความยาวของหน้า) -->
      <section class="bg-white rounded-lg shadow-xl p-8 mb-12 border border-gray-200">
        <h3 class="text-2xl font-semibold text-gray-800 mb-6">จุดเด่นของโปรเจกต์</h3>
        <p class="text-gray-700 leading-relaxed mb-6">
          การดำเนินงานเริ่มจากการนำโปรเจกต์ต้นฉบับ esp32_nat_router มาปรับปรุง โดยเพิ่มฟังก์ชันสำคัญ ได้แก่ DNS Hijack, Captive Portal Server, Temporary NAT Control, และ User Event Handler เพื่อควบคุมการทำงานของ LCD, Buzzer และ MQTT เมื่อมีการ Login สำเร็จ ESP32 จะทำหน้าที่เป็นทั้ง SoftAP, NAT Router, DNS Server และ HTTP Server ที่แสดงหน้าเว็บ Captive Portal สำหรับ Login ด้วย LINE LIFF นอกจากนี้ ยังมีการพัฒนา Backend บน Node.js เพื่อเชื่อมต่อกับ LINE Developer Platform ทำหน้าที่รับ Callback จาก LINE, ขอ Access Token, ดึงข้อมูลผู้ใช้ และส่งข้อมูลกลับมายัง ESP32 ผ่าน MQTT 
        </p>
      </section>

      <!-- Project Details Section 3 (เพิ่มความยาวของหน้า) -->
      <section class="bg-white rounded-lg shadow-xl p-8 mb-12 border border-gray-200">
        <h3 class="text-2xl font-semibold text-gray-800 mb-2">อุปกรณ์และเทคโนโลยีที่ใช้</h3>
        <div class="px-8 py-2">
          <li>ESP-IDF (Espressif IoT Development Framework) v4.4.6</li>
          <li>xtensa-esp32-elf toolchain</li>
          <li>esp_wifi.h, esp_netif.h, esp_event.h – for Wifi</li>
          <li>lwip_napt.h, ip_addr.h – for NAT</li>
          <li>sockets.h, dns_hijack – for DNS</li>
          <li>dhcpserver.h, dhcpserver_options.h – for DHCP</li>
          <li>esp_http_server.h – for HTTP</li>
          <li>I2C Driver</li>
          <li>MQTT Client</li>
          <li>FreeRTOS</li>
          <li>NVS (Non-Volatile Storage)</li>
          <li>Ngrok</li>
          <li>Line platform</li>
        </div>

        
        <h3 class="text-2xl font-semibold text-gray-800 mb-2">ขั้นตอนการทำงาน</h3>
        <div class="px-2 py-2">
          <ol class="list-decimal list-inside text-gray-700 leading-relaxed space-y-2 pl-4">
            <li>เริ่มต้นการทำงาน ESP32 เชื่อมต่อกับ STA และเริ่มทำงานเป็น access point หน้าจอ LCD แสดงข้อความ “Router starting…” buzzer ส่งเสียงดัง ปิ๊บๆ </li>
            <li>เปิด NAT และ DNS hijack หน้าจอ LCD แสดงข้อความ “Network Ready” , “PongPhonpon Wifi” buzzer ส่งเสียงดัง ปิ๊บๆ </li>
            <li>Captive portal server เริ่มการทำงาน</li>
            <li>ผู้ใช้เชื่อมต่อ Wifi DNS ตอบทุก domain ชี้ไปที่ 192.168.4.1 </li>
            <li>แสดงหน้า login ใน captive portal</li>
            <li>ESP32 เปิด NAT ชั่วคราว 3 นาที เพื่อให้ผู้ใช้ออกเน็ตได้ </li>
            <li>ผู้ใช้กด Login Line และถูก redirect ไปที่ browser จริงก่อนระบบจะเปิดแอพ line ขึ้นมาให้ผู้ใช้ login </li>
            <li>ผู้ใช้กดปุ่ม login line และเพิ่มเพื่อน LINE OA ของร้าน จากนั้นระบบแสดงหน้า chat ของ LINE OA </li>
            <li>Backend ส่งข้อมูลผู้ใช้ขึ้นไปที่ MQTT broker </li>
            <li>ESP32 รับ MQTT message แล้วตรวจสอบข้อมูลผู้ใช้ </li>
            <li>LCD แสดงข้อความ “username”, “Friend added!” และ buzzer ส่งเสียง ปิ๊บๆ </li>
            <li>ESP32 เปิด NAT ถาวรให้ผู้ใช้และจบการทำงาน </li>
          </ol>
        </div>

        <h3 class="text-2xl font-semibold text-gray-800 mb-2">ปัญหาและวิธีการแก้ไข</h3>
        <div class="px-2 py-2">
          <ol class="list-decimal list-inside text-gray-700 leading-relaxed space-y-2 pl-4">
            <li>ใน project ต้นฉบับเมื่อ ESP32 ถูกเชื่อมต่อ NAT จะถูกเปิดขึ้นทันที แต่ยังไม่มีฟังก์ชัน DNS hijack เมื่อโทรศัพท์ขอ connectivitycheck.gstatic.com ESP32 จะไม่รู้จัก domain ทำให้ไม่มีการตอบสถานะใดๆกลับมา โทรศัพท์จึงคิดว่าไม่มีอินเทอร์เน็ต แต่ก็ไม่เด้งหน้า captive portal ขึ้นมาให้ การแก้ปัญหา : ทำการเพิ่มฟังก์ชัน DNS hijack ให้ตอบ DNS ทุก domain ให้ช้ำที่ local host ของ ESP32 ก็คือ 192.168.4.1 และเพิ่ม HTTP server ที่จะตอบ HTML ธรรมดาแทน 204 เพื่อให้ captive portal สามารถแสดงขึ้นมาได้</li>
            <li>การแสดงหน้า portal เมื่อผู้ใช้เชื่อมต่อ Wifi จะเป็นหน้า local host ของ ESP32 ทำให้ขณะนั้นผู้ใช้ไม่มีอินเทอร์เน็ต แต่การ login ด้วย line จะต้องมีการ redirect ไปที่ LINE ซึ่งผู้ใช้จะต้องมีความสามารถในการใช้งานอินเทอร์เน็ตได้ในขณะนั้น ทำให้ระบบเกิด Deadlock ESP32 ไม่ได้รับข้อมูลเพราะผู้ใช้ยังไม่ได้ login และผู้ใช้ก็ยังไม่สามารถ login ได้ เพราะยังไม่มีอินเทอร์เน็ต การแก้ปัญหา : เพิ่มการทำงานให้ ESP32 ที่ทำหน้าที่เป็น access point เปิดใช้งาน NAT ชั่วคราวเป็นเวลา 3 นาที เมื่อมีผู้ใช้เชื่อมต่อเข้ามาและกด login ในหน้า captive portal เพื่อให้ผู้ใช้สามารถใช้งานอินเทอร์เน็ตในการ redirect ไปที่ LINE แล้วทำการ login และส่งข้อมูลกลับมาที่ ESP32 เพื่อให้สามารถใช้งานอินเทอร์เน็ตได้ถาวร หากผู้ใช้ไม่ login ภายในเวลาที่กำหนด NAT จะถูกปิดทำให้เครื่องของผู้ใช้ไม่สามารถใช้งานอินเทอร์เน็ตได้</li>
            <li>ปัญหาการสื่อสารกันระหว่าง ESP32 กับ Backend Line callback คือ เมื่อผู้ใช้ทำการ login สำเร็จจะต้องมีการส่งข้อมูลจาก backend กลับมาบอกที่ ESP32 ว่าการ login สำเร็จแล้วเพื่อให้ ESP32  เปิด NAT ให้ผู้ใช้สามารถใช้งานได้ถาวร แต่ backend ซึ่งอยู่ใน public ip กับ ESP32 ที่อยู๋ใน 132.168.4.1 ซึ่งเป็น local host ของตัวเองมันไม่สามารถติดต่อสื่อสารกันได้ ทำให้การ login จะถือว่าไม่สำเร็จ การแก้ปัญหา : ใช้ protocol MQTT แทนการติดต่อกันโดยตรง โดยสร้าง topic ขึ้นมาสำหรับใช้ public ข้อมูลขึ้นไปเมื่อผู้ใช้ login สำเร็จ และให้ ESP32 คอย subscribe topic นั้นไว้เมื่อมี mac address ของเครื่องที่ขอเชื่อมต่อในตอนแรกกลับมาพร้อมกับข้อมูล line ESP32 ก็จะรู้ได้ว่าการ login นั้นสำเร็จแล้ว และจะทำการเปิดอินเทอร์เน็ตให้ใช้งานต่อไปได้</li>
            <li>ปัญหาของฝั่ง backend เมื่อ ESP32 ส่งค่าหลังจากที่ผู้ใช้ทำการ log in สำเร็จแล้ว แต่ backend ไม่สามารถรับค่านั้นมาประมวลผลเพื่อนำไปทำงานในขั้นตอนถัดไปได้ เนื่องมาจากการตั้งค่ารูปแบบของข้อมูลของทั้งฝั่งสองไม่ตรงกัน ในฝั่ง ESP32 ส่งแอททริบิวต์ name แต่ backend รับเป็นรูปแบบแอททริบิวต์ username ซึ่งทำให้รูปแบบของข้อมูลไม่ตรงกัน ฝั่ง backend จึงไม่สามารถรับข้อมูลได้ การแก้ปัญหา : ตั้งค่ารูปแบบของข้อมูลให้สอดคล้องกัน โดยผู้พัฒนาฝั่ง backend ต้องตรวจสอบการตั้งค่ารูปแบบข้อมูลที่จะส่งภายในโค้ดของฝั่ง ESP32</li>
          </ol>
        </div>
        
        <div class="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 gap-6">
            <div class="flex items-center space-x-3 bg-gray-50 p-4 rounded-md shadow-sm">
                <img src={favicon} alt="Svelte" class="w-8 h-8"/>
                <span class="font-medium text-gray-800">Svelte</span>
            </div>
            <div class="flex items-center space-x-3 bg-gray-50 p-4 rounded-md shadow-sm">
                <img src="https://img.icons8.com/color/48/000000/nodejs.png" alt="Node.js" class="w-8 h-8"/>
                <span class="font-medium text-gray-800">Node.js</span>
            </div>
            <div class="flex items-center space-x-3 bg-gray-50 p-4 rounded-md shadow-sm">
                <img src="https://img.icons8.com/color/48/000000/mongodb.png" alt="MongoDB" class="w-8 h-8"/>
                <span class="font-medium text-gray-800">MongoDB</span>
            </div>
            <div class="flex items-center space-x-3 bg-gray-50 p-4 rounded-md shadow-sm">
                <img src={tail} alt="Tailwind CSS" class="w-8 h-8"/>
                <span class="font-medium text-gray-800">Tailwind CSS</span>
            </div>

        </div>
      </section>

      <!-- Project Details Section 4 (เพิ่มความยาวของหน้า) -->
      <section class="bg-white rounded-lg shadow-xl p-8 mb-12 border border-gray-200">
        <h3 class="text-2xl font-semibold text-gray-800 mb-6">ผู้พัฒนา</h3>
        <div class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6">
            <div class="flex items-center space-x-4 bg-gray-50 p-4 rounded-md shadow-sm">
                <img class="h-16 w-16 rounded-full object-cover" src={p1} alt="Team Member 1">
                <div>
                    <div class="text-lg font-semibold text-[#0095ff]">Frontender</div>
                    <div class="text-lg font-semibold text-gray-900">นายปองพล บุรีรักษ์</div>
                    <div class="text-sm text-gray-600">67070100</div>
                </div>
            </div>
            <div class="flex items-center space-x-4 bg-gray-50 p-4 rounded-md shadow-sm">
                <img class="h-16 w-16 rounded-full object-cover" src={p2} alt="Team Member 2">
                <div>
                    <div class="text-lg font-semibold text-[#0095ff]">ESP32 Boarder</div>
                    <div class="text-lg font-semibold text-gray-900">นายพิภพ ทองอั้น</div>
                    <div class="text-sm text-gray-600">67070118</div>
                </div>
            </div>
            <div class="flex items-center space-x-4 bg-gray-50 p-4 rounded-md shadow-sm">
                <img class="h-16 w-16 rounded-full object-cover" src={p3} alt="Team Member 3">
                <div>
                    <div class="text-lg font-semibold text-[#0095ff]">Line Manager</div>
                    <div class="text-lg font-semibold text-gray-900">นายยศพนธ์ มโนวรกุล</div>
                    <div class="text-sm text-gray-600">67070145</div>
                </div>
            </div>
            <div class="flex items-center space-x-4 bg-gray-50 p-4 rounded-md shadow-sm">
                <img class="h-16 w-16 rounded-full object-cover" src={p4} alt="Team Member 3">
                <div>
                    <div class="text-lg font-semibold text-[#0095ff]">Backender</div>
                    <div class="text-lg font-semibold text-gray-900">นายวิรยบวร บุญเปรี่ยม</div>
                    <div class="text-sm text-gray-600">67070166</div>
                </div>
            </div>
        </div>
      </section>

      <!-- เพิ่มส่วนอื่นๆ เพื่อให้หน้ายาวขึ้น -->
      <section class="bg-white rounded-lg shadow-xl p-8 mb-12 border border-gray-200">
        <h3 class="text-2xl font-semibold text-gray-800 mb-6">ข้อดีและข้อจำกัด</h3>
        <div class="grid grid-cols-1 md:grid-cols-2 gap-8">
            <div>
                <h4 class="text-xl font-medium text-green-700 mb-3">ข้อดี (Advantages)</h4>
                <ul class="list-disc list-inside text-gray-700 space-y-1 pl-4 leading-relaxed">
                    <li>เพิ่มความสะดวกและประสบการณ์ที่ดี โดยผู้ใช้ไม่ต้องกรอก username/password แค่กด login ด้วย line ก็สามารถเข้าถึงอินเทอร์เน็ตได้ทันที</li>
                    <li>ระบบจะได้ userid จากการ login ของลูกค้าและทำให้สามารถเพิ่มเพื่อนอันโนมัติใน LINE Official Account</li>
                    <li>ยกระดับ wifi free เป็นช่องทางทำการตลาดที่มีฐานข้อมูลผู้ใช้จริง</li>
                    <li>ร้านค้าสามารถส่งโปรโมชั่น, คูปอง หรือข่าวสารต่างๆให้ลูกค้าทั้งหมดหรือสามารถเลือกส่งเฉพาะบุคคลได้ เป็นการลดภาระของพนักงานในการแนะนำโปรโมชั่นทั้งหมดของร้าน</li>
                    <li>ลูกค้าสามารถติดต่อสื่อสารกับร้านค้าโดยตรงได้ผ่าน LINE OA ของร้าน</li>
                    <li>ร้านค้าสามารถนำข้อมูลจาก ESP32 คือ userid และ mac address มาวิเคราะห์จำนวนผู้ใช้ ความถี่ในการเข้าใช้งาน หรือพฤติกรรมของลูกค้า ช่วยให้ร้านค้าสามารถวิเคราะห์ลูกค้าประจำร้านและสร้างระบบสะสมแต้มหรือส่วนลดได้</li>
                    <li>ช่วยขยายความสามารถของ ESP32 ให้เป็น Gateway อัจฉริยะที่สามารถทำหน้าที่เป็น Nat router, DNS server, HTTP captive portal, Display controller และ Notification System</li>
                    <li>ลดต้นทุนในการสร้างระบบ captive portal สำหรับร้านค้าขนาดเล็ก ใช้เพียงบอร์ด ESP32 ติดตั้งง่าย ใช้งานได้ทันที ไม่ต้องซื้อ router หรือ License Portal Server ราคาแพง</li>
                </ul>
            </div>
            <div>
                <h4 class="text-xl font-medium text-red-700 mb-3">ข้อจำกัด (Limitations)</h4>
                <ul class="list-disc list-inside text-gray-700 leading-relaxed space-y-2 pl-4">
                    <li>เมื่อมือถือเปิด captive portal ขึ้นมาจะเป็นลักษณะของ sandbox ซึ่งปิดการเรียก app จากภายนอกทำให้ไม่สามารถที่จะเปิด line:// ได้และไม่สามารถเปิด OAuth ที่ต้อง redirect ออกไปที่ access.line.me ได้ทำให้เมื่อกดปุ่ม login with line จะค้างอยู่ที่หน้าเดิมการแก้ปัญหา : เพิ่มการ redirect ไปที่ browser จริงก่อน โดยเมื่อผู้ใช้กดปุ่ม login with line หน้า captive portal จะถูกปิดและจะ redirect ไปที่ browser จริงแทน ซึ่งในหน้า browser จะสามารเรียกแอพจากภายนอกได้ ทำให้ line จะถูกเปิดขึ้นมาให้ผู้ใช้ทำการ login ให้เสร็จสิ้นได้</li>
                    <li>เมื่อใช้ ESP32 ในโหมด SoftAP และ Station พร้อมกัน อุปกรณ์จะต้องทั้งเชื่อมต่อเราเตอร์หลักเพื่อออกอินเทอร์เน็ต และปล่อยสัญญาณ Wi-Fi ให้ลูกข่ายในเวลาเดียวกัน แต่เนื่องจาก ESP32 มีวิทยุ Wi-Fi เพียงตัวเดียว จึงต้องแบ่งเวลาในการสื่อสารระหว่างสองหน้าที่นี้ ทำให้แบนด์วิดธ์ที่มีอยู่ถูกแบ่งออก และความเร็วอินเทอร์เน็ตของผู้ใช้ที่เชื่อมต่อผ่าน ESP32 ลดลง</li>
                </ul>
            </div>
        </div>
      </section>

      <!-- ส่วนปิดท้าย (Footer) สามารถเพิ่มได้อีก -->
      <footer class="text-center py-10 text-gray-600">
        <p>&copy; 3P PongPhonPon. All rights reserved.</p>
        <p>Assoc. Prof. Dr. Panwit Tuwanut</p>
      </footer>

    </div>
  </main>
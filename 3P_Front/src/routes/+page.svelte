<script>
    import Mynav from '../components/nav.svelte'
    import Test from '../components/test.svelte';
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
        <h3 class="text-2xl font-semibold text-gray-800 mb-6">รายละเอียด</h3>
        <p class="text-gray-700 leading-relaxed mb-6">
          Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip.
          Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
          Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?
        </p>

        <!-- Image Carousel/Gallery Placeholder -->
        <div class="relative w-full h-80 bg-gray-200 rounded-lg flex items-center justify-between p-4 mb-8">
          <div class="flex-grow flex items-center justify-center">
            <p class="text-gray-500">Image Carousel Placeholder</p>
          </div>
        </div>
        <div class="flex justify-center space-x-4">
          <div class="w-20 h-12 bg-gray-300 rounded-md cursor-pointer hover:bg-gray-400 transition-colors duration-200"></div>
          <div class="w-20 h-12 bg-gray-300 rounded-md cursor-pointer hover:bg-gray-400 transition-colors duration-200"></div>
          <div class="w-20 h-12 bg-gray-300 rounded-md cursor-pointer hover:bg-gray-400 transition-colors duration-200"></div>
        </div>
      </section>

      <!-- Project Details Section 2 (เพิ่มความยาวของหน้า) -->
      <section class="bg-white rounded-lg shadow-xl p-8 mb-12 border border-gray-200">
        <h3 class="text-2xl font-semibold text-gray-800 mb-6">จุดเด่นของโปรเจกต์</h3>
        <p class="text-gray-700 leading-relaxed mb-6">
          Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip.
          Curabitur pretium tincidunt lacus. Nulla gravida orci a odio. Nullam varius, turpis et commodo pharetra, est eros bibendum elit, nec luctus magna felis sollicitudin mauris. In porta dapibus egestas.
          Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. In non orci eu purus porttitor egestas. Praesent dapibus, neque id cursus faucibus, tortor neque egestas augue, eu vulputate magna eros eu erat.
          Aliquam erat volutpat. Nam dui mi, tincidunt quis, accumsan porttitor, facilisis luctus, metus. Nulla facilisi. Proin volutpat sem at metus egestas non tristique nunc scelerisque. Ut at ligula quis purus dignissim molestie. Nam mattis, libero in feugiat.
          Morbi vitae felis nec lectus pharetra dictum. Cras in velit vitae elit imperdiet bibendum. Nunc elementum dui vitae lectus lacinia ac tristique ante pulvinar. Ut non elit a mi ullamcorper.
        </p>
        <ul class="list-disc list-inside text-gray-700 leading-relaxed space-y-2 pl-4">
            <li>Feature 1: Enhanced User Experience</li>
            <li>Feature 2: Scalable Architecture</li>
            <li>Feature 3: Robust Security Measures</li>
            <li>Feature 4: Seamless Integration</li>
            <li>Feature 5: Comprehensive Reporting</li>
            <li>Feature 6: Multi-language Support</li>
            <li>Feature 7: Automated Workflows</li>
            <li>Feature 8: Real-time Analytics Dashboard</li>
        </ul>
      </section>

      <!-- Project Details Section 3 (เพิ่มความยาวของหน้า) -->
      <section class="bg-white rounded-lg shadow-xl p-8 mb-12 border border-gray-200">
        <h3 class="text-2xl font-semibold text-gray-800 mb-6">เทคโนโลยีที่ใช้</h3>
        <p class="text-gray-700 leading-relaxed mb-6">
          Fusce non enim ut enim consectetur fermentum. Aenean vitae arcu ut ipsum blandit ultrices.
          Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Sed dictum sapien quis felis ullamcorper, vel scelerisque orci faucibus.
          Maecenas id nisi eu mi efficitur convallis vel nec nunc. Etiam facilisis est a massa tincidunt volutpat.
          Donec bibendum ex vitae eros bibendum, eu pharetra turpis viverra.
          Nulla facilisi. Integer vel nisl nec tortor convallis ullamcorper. In hac habitasse platea dictumst.
        </p>
        <div class="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 gap-6">
            <div class="flex items-center space-x-3 bg-gray-50 p-4 rounded-md shadow-sm">
                <img src="https://img.icons8.com/color/48/000000/react.png" alt="React" class="w-8 h-8"/>
                <span class="font-medium text-gray-800">React.js</span>
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
                <img src="https://img.icons8.com/color/48/000000/tailwind-css.png" alt="Tailwind CSS" class="w-8 h-8"/>
                <span class="font-medium text-gray-800">Tailwind CSS</span>
            </div>
            <div class="flex items-center space-x-3 bg-gray-50 p-4 rounded-md shadow-sm">
                <img src="https://img.icons8.com/color/48/000000/docker.png" alt="Docker" class="w-8 h-8"/>
                <span class="font-medium text-gray-800">Docker</span>
            </div>
            <div class="flex items-center space-x-3 bg-gray-50 p-4 rounded-md shadow-sm">
                <img src="https://img.icons8.com/color/48/000000/amazon-web-services.png" alt="AWS" class="w-8 h-8"/>
                <span class="font-medium text-gray-800">AWS</span>
            </div>
        </div>
      </section>

      <!-- Project Details Section 4 (เพิ่มความยาวของหน้า) -->
      <section class="bg-white rounded-lg shadow-xl p-8 mb-12 border border-gray-200">
        <h3 class="text-2xl font-semibold text-gray-800 mb-6">ทีมผู้พัฒนา</h3>
        <div class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6">
            <div class="flex items-center space-x-4 bg-gray-50 p-4 rounded-md shadow-sm">
                <img class="h-16 w-16 rounded-full object-cover" src="https://images.unsplash.com/photo-1570295999919-56ceb5ecca61?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8&auto=format&fit=facearea&facepad=4&w=144&h=144&q=80" alt="Team Member 1">
                <div>
                    <div class="text-lg font-semibold text-gray-900">John Doe</div>
                    <div class="text-sm text-gray-600">Project Lead</div>
                </div>
            </div>
            <div class="flex items-center space-x-4 bg-gray-50 p-4 rounded-md shadow-sm">
                <img class="h-16 w-16 rounded-full object-cover" src="https://images.unsplash.com/photo-1494790108377-be9c29b29329?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8&auto=format&fit=facearea&facepad=4&w=144&h=144&q=80" alt="Team Member 2">
                <div>
                    <div class="text-lg font-semibold text-gray-900">Jane Smith</div>
                    <div class="text-sm text-gray-600">Frontend Developer</div>
                </div>
            </div>
            <div class="flex items-center space-x-4 bg-gray-50 p-4 rounded-md shadow-sm">
                <img class="h-16 w-16 rounded-full object-cover" src="https://images.unsplash.com/photo-1500648767791-00dcc994a43e?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8&auto=format&fit=facearea&facepad=4&w=144&h=144&q=80" alt="Team Member 3">
                <div>
                    <div class="text-lg font-semibold text-gray-900">Peter Jones</div>
                    <div class="text-sm text-gray-600">Backend Developer</div>
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
                <ul class="list-disc list-inside text-gray-700 leading-relaxed space-y-2 pl-4">
                    <li>Improved efficiency in promotion management.</li>
                    <li>Centralized customer data for targeted campaigns.</li>
                    <li>User-friendly interface for easy navigation.</li>
                    <li>Scalable to accommodate future growth.</li>
                    <li>Real-time tracking of promotion performance.</li>
                </ul>
            </div>
            <div>
                <h4 class="text-xl font-medium text-red-700 mb-3">ข้อจำกัด (Limitations)</h4>
                <ul class="list-disc list-inside text-gray-700 leading-relaxed space-y-2 pl-4">
                    <li>Requires initial data migration from existing systems.</li>
                    <li>Potential learning curve for new users.</li>
                    <li>Dependency on third-party integrations for advanced features.</li>
                    <li>Performance may vary with extremely large datasets.</li>
                    <li>Customization options are limited to defined parameters.</li>
                </ul>
            </div>
        </div>
      </section>

      <!-- ส่วนปิดท้าย (Footer) สามารถเพิ่มได้อีก -->
      <footer class="text-center py-10 text-gray-600">
        <p>&copy; 3P PongPhonPon. All rights reserved.</p>
        <p>ชื่ออาจารย์</p>
      </footer>

    </div>
  </main>
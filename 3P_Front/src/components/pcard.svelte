<script lang="ts">
    import { coupon } from "../coupon.svelte";

    function updateCouponName() {
        coupon.name = ( document.getElementById('promotion-name') as HTMLInputElement).value;
    }

    function updateCouponDetails() {
        coupon.details = ( document.getElementById('promotion-information') as HTMLInputElement).value;
    }

    function updateCouponStartDate() {
        coupon.startDate = ( document.getElementById('start-date') as HTMLInputElement).value;
    }

    function updateCouponEndDate() {
        coupon.endDate = ( document.getElementById('end-date') as HTMLInputElement).value;
    }

    // $inspect(coupon);
</script>


<div class="max-w-md bg-white p-6 rounded-lg shadow-lg relative z-10">
        <label for="promotion-name" class="block text-gray-700 mb-2 font-bold">Promotion</label>
        <input type="text" id="promotion-name" placeholder="กรอกชื่อโปรโมชั่น" onfocusout={() => updateCouponName()}
        class="
            w-full
            p-3
            border border-gray-300
            rounded-lg
            focus:ring-blue-500 focus:border-blue-500
            outline-none
            transition-all" />
        <label for="promotion-information" class="block text-gray-700 mb-2 mt-2 font-bold">Details</label>
        <textarea name="promotion-information" id="promotion-information" rows="8" placeholder="กรอกรายละเอียดโปรโมชั่น" onfocusout={() => updateCouponDetails()}
        class="
            w-full
            p-3
            border border-gray-300
            rounded-lg
            focus:ring-blue-500 focus:border-blue-500
            outline-none
            transition-all
            resize-none
        "></textarea>
        <div>

            <label for="start-date end-date" class="block text-gray-700 font-medium mb-2">วันเริ่มต้น และ หมดอายุ</label>
            <input id="start-date" name="start-date" type="date" oninput={() => updateCouponStartDate()}
            class="
                flex-1
                p-1
                pl-2
                text-sm
                border border-gray-300
                rounded-full
                text-center
                focus:ring-blue-500 focus:border-blue-500
                outline-none
                bg-blue-50
                text-blue-700
                font-medium
                transition-all
            "/>
            <span class="text-gray-500">–</span>
            <input id="end-date" name="end-date" type="date" oninput={() => updateCouponEndDate()}
            class="
                flex-1
                p-1
                pl-2
                text-sm
                border border-gray-300
                rounded-full
                text-center
                focus:ring-blue-500 focus:border-blue-500
                outline-none
                bg-blue-50
                text-blue-700
                font-medium
                transition-all
            "/>
        </div>
</div>

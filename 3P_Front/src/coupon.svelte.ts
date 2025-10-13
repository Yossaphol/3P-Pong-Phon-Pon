interface coupon {
    name: string;
    details: string;
    startDate: string;
    endDate: string;
}

export let coupon: coupon = $state({
    name: '',
    details: '',
    startDate: '',
    endDate: ''
});
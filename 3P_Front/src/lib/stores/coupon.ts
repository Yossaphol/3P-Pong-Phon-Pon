import { writable } from "svelte/store";

export interface Coupon {
  name: string;
  details: string;
  startDate: string;
  endDate: string;
}

export const coupon = writable<Coupon>({
  name: "",
  details: "",
  startDate: "",
  endDate: ""
});

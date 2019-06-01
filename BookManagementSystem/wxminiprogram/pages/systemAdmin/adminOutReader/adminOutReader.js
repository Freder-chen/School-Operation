// pages/adminOutReader/adminOutReader.js
const { $Message } = require('../../../lib/base/index');
const app = getApp();

Page({

  /**
   * 页面的初始数据
   */
  data: {
    user_name: "",
    // user data
    id: "",
    name: "",
    departments: "",
    major: "",
    phone: "",
    email: "",
    borrow_limit: "",
    borrow_period: "",
    borrow_amount: "",
  },

  getReaderClick: function () {
    var that = this
    wx.request({
      url: app.globalData.post_url + 'get-user',
      method: "POST",
      header: app.globalData.header,
      data: {
        id: that.data.id,
      },
      success: function (res) {
        let jdata = res.data;
        if (jdata.status == "success") {
          that.setData({
            name: jdata.data.name,
            departments: jdata.data.departments,
            major: jdata.data.major,
            phone: jdata.data.phone,
            email: jdata.data.email,
            borrow_limit: jdata.data.borrow_limit,
            borrow_period: jdata.data.borrow_period,
            borrow_amount: jdata.data.borrow_amount
          })
        } else {
          $Message({ content: jdata.status });
        }
      },
      fail: function (res) {
        $Message({ content: '异常' });
      },
    })
  },

  delReaderClick: function() {
    wx.request({
      url: app.globalData.post_url + 'del-user',
      method: "POST",
      header: app.globalData.header,
      data: {
        id: this.data.id,
      },
      success: function (res) {
        let jdata = res.data
        $Message({ content: jdata.status });
      },
      fail: function (res) {
        $Message({ content: '异常' });
      },
    })
  },

  bindIdInput: function ({ detail }) { this.setData({ id: detail.detail.value }) },
  bindNameInput: function ({ detail }) { this.setData({ name: detail.detail.value }) },
  bindDepartmentsInput: function ({ detail }) { this.setData({ departments: detail.detail.value }) },
  bindMajorInput: function ({ detail }) { this.setData({ major: detail.detail.value }) },
  bindPhoneInput: function ({ detail }) { this.setData({ phone: detail.detail.value }) },
  bindEmailInput: function ({ detail }) { this.setData({ email: detail.detail.value }) },
  bindBorrowLimitInput: function ({ detail }) { this.setData({ borrow_limit: detail.detail.value }) },
  bindBorrowPeriodInput: function ({ detail }) { this.setData({ borrow_period: detail.detail.value }) },
  bindBorrowAmountInput: function ({ detail }) { this.setData({ borrow_amount: detail.detail.value }) },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})
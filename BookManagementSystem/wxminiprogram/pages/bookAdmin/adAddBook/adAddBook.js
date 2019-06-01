// pages/bookAdmin/adAddBook/adAddBook.js

const { $Message } = require('../../../lib/base/index');
const app = getApp();

Page({

  /**
   * 页面的初始数据
   */
  data: {
    id: "",
    name: "",
    author: "",
    translator: "",
    price: "",
    isbnCode: "",
    publishCompany: "",
    comeUpTime: "",
    enteringMen: ""
  },

  addBookClick: function () {
    wx.request({
      url: app.globalData.post_url + 'add-book',
      method: "POST",
      header: app.globalData.header,
      data: {
        id: this.data.id,
        name: this.data.name,
        author: this.data.author,
        translator: this.data.translator,
        price: this.data.price,
        isbn_code: this.data.isbnCode,
        publish_company: this.data.publishCompany,
        come_up_time: this.data.comeUpTime,
        entering_men: this.data.enteringMen,
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
  bindAuthorInput: function ({ detail }) { this.setData({ author: detail.detail.value }) },
  bindTranslatorInput: function ({ detail }) { this.setData({ translator: detail.detail.value }) },
  bindPriceInput: function ({ detail }) { this.setData({ price: detail.detail.value }) },
  bindIsbnCodeInput: function ({ detail }) { this.setData({ isbnCode: detail.detail.value }) },
  bindPublishCompanyInput: function ({ detail }) { this.setData({ publishCompany: detail.detail.value }) },
  bindComeUpTimeInput: function ({ detail }) { this.setData({ comeUpTime: detail.detail.value }) },
  bindEnteringMenInput: function ({ detail }) { this.setData({ enteringMen: detail.detail.value }) },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    var that = this
    wx.request({
      url: app.globalData.post_url + 'generate-book-id',
      method: "POST",
      header: app.globalData.header,
      data: {},
      success: function (res) {
        let jdata = res.data;
        if (jdata.status == "success") {
          that.setData({ id: jdata.data })
        } else {
          $Message({ content: jdata.status });
        }
      },
      fail: function (res) {
        $Message({ content: '异常' });
      },
    })
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
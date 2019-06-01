// pages/adBorrowBook/adBorrowBook.js
const { $Message } = require('../../../lib/base/index');
const app = getApp();

Page({

  /**
   * 页面的初始数据
   */
  data: {
    userId: "",
    bookId: "",

    tableKeyList: ['借阅号', '图书编号', '应还日期'],
    tableDataList: []
  },

  findUserBorrowingClick: function () {
    var that = this
    console.log(this.data.userId)
    wx.request({
      url: app.globalData.post_url + 'find-user-borrowing',
      method: "POST",
      header: app.globalData.header,
      data: { user_id: that.data.userId },
      success: function (res) {
        let jdata = res.data;
        console.log(jdata)
        if (jdata.status == "success") {
          that.setData({ tableDataList: jdata.data })
          $Message({ content: jdata.status });
        } else {
          $Message({ content: jdata.status });
        }
      },
      fail: function (res) {
        $Message({ content: '异常' });
      },
    })
  },

  borrowBookClick: function () {
    wx.request({
      url: app.globalData.post_url + 'borrow-book',
      method: "POST",
      header: app.globalData.header,
      data: {
        user_id: this.data.userId,
        book_id: this.data.bookId,
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

  bindUserIdInput: function ({ detail }) { this.setData({ userId: detail.detail.value }) },
  bindBookIdInput: function ({ detail }) { this.setData({ bookId: detail.detail.value }) },

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